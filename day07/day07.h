#pragma once
#include "../myhelpers.h"
#include <map>
struct Hand {
  std::string cards;
  uint32_t strength;
  uint32_t bid;
};

enum HandType {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
};

HandType getHandType(std::string cards) {
  std::map<char, uint8_t> cardCounts;
  for (auto c : cards)
    cardCounts[c]++;

  uint8_t highestCount = 0;
  uint8_t secondHighestCount = 0;
  char highestCard = ' ';
  for (auto it = cardCounts.begin(); it != cardCounts.end(); it++) {
    if (it->second > highestCount) {
      highestCount = it->second;
      highestCard = it->first;
    }
  }
  for (auto it = cardCounts.begin(); it != cardCounts.end(); it++) {
    if (it->first != highestCard)
        secondHighestCount = std::max(secondHighestCount, it->second);
  }
  if (highestCount == 5)
    return FIVE_OF_A_KIND;
  else if (highestCount == 4)
    return FOUR_OF_A_KIND;
  else if (highestCount == 3 && secondHighestCount == 2)
    return FULL_HOUSE;
  else if (highestCount == 2 && secondHighestCount == 2)
    return TWO_PAIR;
  else if (highestCount == 3)
    return THREE_OF_A_KIND;
  else if (highestCount == 2)
    return ONE_PAIR;
  else
    return HIGH_CARD;
}

void day07() {
  // order hands based on strength
  // first card = index 0, second = index 1
  // determine strongest hand (type has priority, followed by strongest first,
  // second, third cards etc) hand rank: 1:n (weakest:strongest) output: total
  // winnings sum (rank of hand * bid amount)
  std::unordered_map<char, uint8_t> cardStrengths{
      {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
      {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

  for (auto line : getLinesFromFile("day07/test.txt")) {
    auto values = splitStrByDelim(line, " ");
    auto cards = values[0];
    auto bid = values[1];
    auto handType = getHandType(cards);
    std::cout << handType << std::endl;
  }
}