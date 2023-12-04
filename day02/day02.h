#pragma once
#include <regex>
#include <unordered_map>
#include "../myhelpers.h"
void day02() {
  std::unordered_map<std::string, int> maxNrOfCubesByColor = {
      {"red", 12}, {"green", 13}, {"blue", 14}};
  int sumOfGameIDs = 0;
  int powerSum = 0;

  for (auto &line : getLinesFromFile("day02/input.txt")) {
    bool isValidForPart1 = true;
    std::string gameID = "";
    std::smatch sm;
    std::regex_search(line, sm, std::regex("(\\d+):"));
    gameID = sm[1];

    std::unordered_map<std::string, int> cubeCounts{
        {"red", 0}, {"green", 0}, {"blue", 0}};
    line = std::regex_replace(line, std::regex("Game \\d+: "), "");
    for (auto &subset : splitStrByDelim(line, "; ")) {
      for (auto &cubePair : splitStrByDelim(subset, ", ")) {
        auto colorCount = splitStrByDelim(cubePair, " ");
        int count = std::stoi(colorCount[0]);

        if (count > cubeCounts[colorCount[1]])
          cubeCounts[colorCount[1]] = count;

        if (count > maxNrOfCubesByColor[colorCount[1]])
          isValidForPart1 = false;
      }
    }
    if (isValidForPart1)
      sumOfGameIDs += std::stoi(gameID);

    int power = cubeCounts["red"] * cubeCounts["green"] * cubeCounts["blue"];
    powerSum += power;
  }

  std::cout << std::to_string(sumOfGameIDs) << std::endl;
  writeToFile(std::to_string(sumOfGameIDs), "day02/output1.txt");

  std::cout << std::to_string(powerSum) << std::endl;
  writeToFile(std::to_string(powerSum), "day02/output2.txt");
}