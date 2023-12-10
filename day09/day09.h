#pragma once
#include "../myhelpers.h"
struct History {
  std::vector<std::vector<int>> values;
  void print() {
    for (auto value : values) {
      for (auto number : value) {
        std::cout << number << " ";
      }
      std::cout << std::endl;
    }
  }
  int getExtrapolatedValue() { return values[0].back(); }
  int getReverseExtrapolatedValue() { return values[0].front(); }
  void populate(std::vector<int> initialHistoryValue) {
    auto history = values.emplace_back(initialHistoryValue);
    // find differences
    while (true) {
      std::vector<int> differences(history.size());
      std::adjacent_difference(history.begin(), history.end(),
                               differences.begin());
      differences.erase(differences.begin());
      history = values.emplace_back(differences);
      bool allZero =
          std::count(history.begin(), history.end(), 0) == history.size();
      if (allZero)
        break;
    }
  }
  void extrapolate() {
    values.back().emplace_back(0);
    for (int i = values.size() - 2; i >= 0; i--) {
      int leftValue = values[i].back();
      int belowValue = values[i + 1].back();
      values[i].emplace_back(leftValue + belowValue);
    }
  }
  void reverseExtrapolate() {
    values.back().emplace(values.back().begin(), 0);
    for (int i = values.size() - 2; i >= 0; i--) {
      int rightValue = values[i].front();
      int belowValue = values[i + 1].front();
      values[i].emplace(values[i].begin(), rightValue - belowValue);
    }
  }
};
void day09() {
  auto lines = aoc::getLinesFromFile("day09/input.txt");
  std::vector<History *> histories;
  for (auto line : lines) {
    std::vector<std::string> values = aoc::splitStrByDelim(line, " ");
    std::vector<int> history;
    History *newHistory = histories.emplace_back(new History());
    for (auto value : values) {
      history.emplace_back(std::stoi(value));
    }

    newHistory->populate(history);
    newHistory->extrapolate();
    newHistory->reverseExtrapolate();
  }
  for (auto history : histories)
    history->print();

  int part1Sum = 0;
  for (auto history : histories) {
    part1Sum += history->getExtrapolatedValue();
  }
  int part2Sum = 0;
  for (auto history : histories) {
    part2Sum += history->getReverseExtrapolatedValue();
  }
  std::cout << part1Sum << std::endl;
  std::cout << part2Sum << std::endl;
  // for (auto diff : differences)
  //   std::cout << diff << " ";
}