#pragma once
#include "../myhelpers.h"
#include <map>

enum HandType {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
};

struct Hand {
  uint64_t part1Strength;
  uint64_t part2Strength;
  uint32_t bid;
  std::string cards;
  HandType part1Type;
  HandType part2Type;
};

HandType getPart1HandType(std::string cards) {
  // J:s are regular cards
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

HandType getPart2HandType(std::string cards) {
  // J:s are special cards
  std::map<char, uint8_t> cardCounts;
  for (auto c : cards)
    cardCounts[c]++;

  uint8_t highestCount = 0;
  uint8_t secondHighestCount = 0;
  char highestCard = ' ';
  char secondHighestCard = ' ';
  for (auto it = cardCounts.begin(); it != cardCounts.end(); it++) {
    if (it->second > highestCount) {
      highestCount = it->second;
      highestCard = it->first;
    }
  }
  for (auto it = cardCounts.begin(); it != cardCounts.end(); it++) {
    if (it->first != highestCard && it->second > secondHighestCount) {
      secondHighestCount = it->second;
      secondHighestCard = it->first;
    }
  }

  uint8_t nrOfJokers = cardCounts['J'];
  switch (nrOfJokers) {
  case 5:
    return FIVE_OF_A_KIND;
  case 4:
    return FIVE_OF_A_KIND;
  case 3:
    if (secondHighestCount == 2)
      return FIVE_OF_A_KIND;
    else
      return FOUR_OF_A_KIND;
  case 2:
    if (highestCount == 3)
      return FIVE_OF_A_KIND;
    else if (highestCount == 2 && secondHighestCount == 2) // 2 jokers, 2 others
      return FOUR_OF_A_KIND;
    else
      return THREE_OF_A_KIND;
  case 1:
    if (highestCount == 4)
      return FIVE_OF_A_KIND;
    else if (highestCount == 3)
      return FOUR_OF_A_KIND;
    else if (highestCount == 2 && secondHighestCount == 2)
      return FULL_HOUSE;
    else if (highestCount == 2)
      return THREE_OF_A_KIND;
    else
      return ONE_PAIR;
  default:
    break;
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
  std::unordered_map<char, uint8_t> part1CardStrengths{
      {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
      {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};
  std::unordered_map<char, uint8_t> part2CardStrengths{
      {'J', 1}, {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6}, {'7', 7},
      {'8', 8}, {'9', 9}, {'T', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};

  std::vector<Hand *> hands;

  for (auto &line : getLinesFromFile("day07/input.txt")) {
    auto values = splitStrByDelim(line, " ");
    Hand *newHand = new Hand();
    std::string cards = values[0];
    std::string bid = values[1];
    newHand->bid = std::stoul(bid);

    HandType part1HandType = getPart1HandType(cards);
    HandType part2HandType = getPart2HandType(cards);

    uint64_t part1HandStrength = part1HandType * 1e18;
    uint64_t part2HandStrength = part2HandType * 1e18;

    uint64_t currentCardIndexStrengthModifier = 1e12;
    for (auto card : cards) {
      part1HandStrength +=
          part1CardStrengths[card] * currentCardIndexStrengthModifier;

      part2HandStrength +=
          part2CardStrengths[card] * currentCardIndexStrengthModifier;

      currentCardIndexStrengthModifier *= 0.01;
    }
    newHand->part1Strength = part1HandStrength;
    newHand->part2Strength = part2HandStrength;
    newHand->cards = cards;
    newHand->part1Type = part1HandType;
    newHand->part2Type = part2HandType;
    hands.emplace_back(newHand);
  }

  std::sort(hands.begin(), hands.end(), [](Hand *a, Hand *b) {
    return a->part1Strength < b->part1Strength;
  });

  // 252263809 too low
  // 252287131 too low
  // 252295678 correct!
  uint64_t part1Sum = 0;
  for (int i = 0; i < hands.size(); i++) {
    part1Sum += ((uint64_t)i + 1) * hands[i]->bid;
    std::string debugStr = std::format(
        "cards: {} | type: {}, strength: {:020} rank: {:04}, bid: {:03}, sum: "
        "{:04}, totalSum: {:09}",
        hands[i]->cards, (uint8_t)hands[i]->part1Type, hands[i]->part1Strength,
        i + 1, hands[i]->bid, i + 1 * hands[i]->bid, part1Sum);
    std::cout << debugStr << std::endl;
  }
  std::cout << part1Sum << std::endl;

  std::sort(hands.begin(), hands.end(), [](Hand *a, Hand *b) {
    return a->part2Strength < b->part2Strength;
  });

  // 251871559 too high
  // 251714671 too high
  // 251618339 too high
  // 251223935 "not the right answer"
  // 250577259 correct!
  uint64_t part2Sum = 0;
  for (int i = 0; i < hands.size(); i++) {
    part2Sum += ((uint64_t)i + 1) * hands[i]->bid;
    std::string debugStr = std::format(
        "cards: {} | type: {}, strength: {:020} rank: {:04}, bid: {:03}, sum: "
        "{:06}, totalSum: {:010}",
        hands[i]->cards, (uint8_t)hands[i]->part2Type, hands[i]->part2Strength,
        i + 1, hands[i]->bid, i + 1 * hands[i]->bid, part2Sum);
    std::cout << debugStr << std::endl;
  }
  std::cout << part2Sum << std::endl;
  writeToFile(std::to_string(part1Sum), "day07/part1.txt");
  writeToFile(std::to_string(part2Sum), "day07/part2.txt");
}