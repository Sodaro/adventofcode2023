#pragma once
#include "../common.h"
#include <regex>
#include <unordered_map>
void day02() {
  std::unordered_map<std::string, int> numberOfCubesByColor = {
      {"red", 12}, {"green", 13}, {"blue", 14}};
  int sumOfGameIDs = 0;
  int powerSum = 0;

  for (auto &line : getLinesFromFile("day02/input.txt")) {
    bool isValidForPart1 = true;
    std::string gameID = "";
    std::smatch sm;
    std::regex_search(line, sm, std::regex("(\\d+):"));
    gameID = sm[1];

    int red = 1, green = 1, blue = 1;
    line = std::regex_replace(line, std::regex("Game \\d+: "), "");
    for (auto &subset : splitStrByDelim(line, "; ")) {
      for (auto &cubePair : splitStrByDelim(subset, ", ")) {
        auto colorCount = splitStrByDelim(cubePair, " ");
        int count = std::stoi(colorCount[0]);

        if (colorCount[1] == "red" && count > red)
          red = count;
        else if (colorCount[1] == "green" && count > green)
          green = count;
        else if (colorCount[1] == "blue" && count > blue)
          blue = count;

        if (count > numberOfCubesByColor[colorCount[1]])
          isValidForPart1 = false;
      }
    }
    if (isValidForPart1)
      sumOfGameIDs += std::stoi(gameID);

    int power = red * green * blue;
    powerSum += power;
  }

  std::cout << std::to_string(sumOfGameIDs) << std::endl;
  writeToFile(std::to_string(sumOfGameIDs), "day02/output1.txt");

  std::cout << std::to_string(powerSum) << std::endl;
  writeToFile(std::to_string(powerSum), "day02/output2.txt");
}