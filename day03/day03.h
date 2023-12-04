#pragma once
#include "../myhelpers.h"
#include <format>
#include <math.h>
#include <numeric>
#include <unordered_map>
#include <vector>

#define NOMINMAX
#include <windows.h>

struct PartNumber {
  PartNumber(int row, int col) : rowIndex(row), minCol(col), maxCol(col){};
  int value = 0;
  int rowIndex = -1;
  int minCol = -1;
  int maxCol = -1;
};

struct Gear {
  int rowIndex = -1;
  int colIndex = -1;
  std::vector<int> adjacentPartNumberValues;
};

void getStartAndEndPositions(int row, int minCol, int maxCol, int length,
                             int &outStartCol, int &outEndCol, int &outStartRow,
                             int &outEndRow) {
  outStartCol = std::max(minCol - 1, 0);
  outEndCol = std::min(maxCol + 1, length - 1);
  outStartRow = std::max(row - 1, 0);
  outEndRow = std::min(row + 1, length - 1);
}

bool hasNeighboringSymbol(const PartNumber *partNumber,
                          const std::vector<std::string> &lines) {

  int startCol, endCol, startRow, endRow;
  getStartAndEndPositions(partNumber->rowIndex, partNumber->minCol,
                          partNumber->maxCol, lines[0].length(), startCol,
                          endCol, startRow, endRow);
  for (int row = startRow; row <= endRow; row++) {
    for (int col = startCol; col <= endCol; col++) {
      if (lines[row][col] != '.' && !isdigit(lines[row][col]))
        return true;
    }
  }

  return false;
}

void findAllAdjacentGears(const PartNumber *partNumber,
                          const std::vector<std::string> &lines,
                          std::vector<std::pair<int, int>> &outGearPositions) {

  int startCol, endCol, startRow, endRow;
  getStartAndEndPositions(partNumber->rowIndex, partNumber->minCol,
                          partNumber->maxCol, lines[0].length(), startCol,
                          endCol, startRow, endRow);
  for (int row = startRow; row <= endRow; row++) {
    for (int col = startCol; col <= endCol; col++) {
      if (lines[row][col] == '*')
        outGearPositions.emplace_back(std::make_pair(row, col));
    }
  }
}

void printCheckedCells(const PartNumber *partNumber,
                       const std::vector<std::string> &lines) {

  int startCol, endCol, startRow, endRow;
  getStartAndEndPositions(partNumber->rowIndex, partNumber->minCol,
                          partNumber->maxCol, lines[0].length(), startCol,
                          endCol, startRow, endRow);

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  bool hasNeighbor = hasNeighboringSymbol(partNumber, lines);
  // make console text red or green
  SetConsoleTextAttribute(hConsole, hasNeighbor ? 10 : 12);

  std::string output = "";
  for (int row = startRow; row <= endRow; row++) {
    for (int col = startCol; col <= endCol; col++) {
      output += lines[row][col];
    }
    output += "\n";
  }
  std::cout << output << std::endl;
  std::cout << std::endl;
  SetConsoleTextAttribute(hConsole, 15);
}

void day03() {
  auto lines = getLinesFromFile("day03/input.txt");
  std::vector<PartNumber *> partNumbers;
  PartNumber *currentPart = nullptr;
  std::unordered_map<std::string, Gear *> gears;
  for (int i = 0; i < lines.size(); i++) {
    std::string currentNumberStr = "";
    for (int j = 0; j < lines[i].length(); j++) {
      if (isdigit(lines[i][j])) {
        if (currentPart == nullptr) {
          currentPart = new PartNumber(i, j);
          currentNumberStr = std::string(1, lines[i][j]);
        } else {
          currentNumberStr += lines[i][j];
          currentPart->maxCol = j;
        }
      }
      if (currentNumberStr != "" &&
          (!isdigit(lines[i][j]) || (j + 1) == lines[i].length())) {
        currentPart->value = std::stoi(currentNumberStr);
        partNumbers.push_back(currentPart);
        currentPart = nullptr;
        currentNumberStr = "";
      }
      // part2
      if (lines[i][j] == '*') {
        std::string key =
            std::format("{0};{1}", std::to_string(i), std::to_string(j));
        gears[key] = new Gear();
        gears[key]->rowIndex = i;
        gears[key]->colIndex = j;
      }
    }
  }

  int sum = std::accumulate(partNumbers.begin(), partNumbers.end(), 0,
                            [&](int sum, const PartNumber *part) {
                              if (hasNeighboringSymbol(part, lines)) {
                                return sum + part->value;
                              } else
                                return sum;
                            });

  for (auto partnr : partNumbers)
    printCheckedCells(partnr, lines);

  std::cout << std::to_string(sum) << std::endl;

  // part2
  for (auto partNumber : partNumbers) {
    std::vector<std::pair<int, int>> adjacentGearPositions;
    findAllAdjacentGears(partNumber, lines, adjacentGearPositions);
    for (auto gearPosition : adjacentGearPositions) {
      std::string key =
          std::format("{0};{1}", std::to_string(gearPosition.first),
                      std::to_string(gearPosition.second));

      if (gears.find(key) != gears.end()) {
        gears[key]->adjacentPartNumberValues.push_back(partNumber->value);
      }
    }
  }

  int gearRatioSum = 0;
  for (auto &[key, value] : gears) {
    if (value->adjacentPartNumberValues.size() == 2) {
      gearRatioSum += value->adjacentPartNumberValues[0] *
                      value->adjacentPartNumberValues[1];
    }
  }
  std::cout << std::to_string(gearRatioSum) << std::endl;
}