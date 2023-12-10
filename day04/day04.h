#pragma once
#include "../myhelpers.h"
void day04() {

  int valueOfCards = 0;
  auto lines = aoc::getLinesFromFile("day04/input.txt");
  std::vector<int> nrOfCopiesOfCards(lines.size(), 0);
  int index = 0;
  for (auto line : lines) {
    std::vector<std::string> cardAndNumbers = aoc::splitStrByDelim(line, ":");
    std::vector<std::string> numberStrings =
        aoc::splitStrByDelim(cardAndNumbers[1], "|");
    std::vector<std::string> winningNumbers =
        aoc::splitStrByDelim(numberStrings[0], " ");
    std::vector<std::string> myNumbers =
        aoc::splitStrByDelim(numberStrings[1], " ");
    int nrOfWinningNumbers = std::count_if(
        myNumbers.begin(), myNumbers.end(), [&winningNumbers](std::string& number) {
          return std::find(winningNumbers.begin(), winningNumbers.end(),
                           number) != std::end(winningNumbers);
        });

    valueOfCards += (int)std::pow(2, nrOfWinningNumbers-1);
    for (int i = 0; i < nrOfWinningNumbers; i++) {
      nrOfCopiesOfCards[index + i + 1] += 1 + nrOfCopiesOfCards[index];
    }
    index++;
  }
  int nrOfCards =
      std::accumulate(nrOfCopiesOfCards.begin(), nrOfCopiesOfCards.end(), 0) + nrOfCopiesOfCards.size();
  std::cout << valueOfCards << std::endl;
  std::cout << nrOfCards << std::endl;
  aoc::writeToFile(std::to_string(valueOfCards), "day04/part1.txt");
  aoc::writeToFile(std::to_string(nrOfCards), "day04/part2.txt");
}