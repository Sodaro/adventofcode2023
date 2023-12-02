#pragma once
#include <regex>
#include <unordered_map>

#include "../common.h"

void day01Part1() {
  int sum = 0;
  for (auto line : getLinesFromFile("day01/part1.txt")) {
    line = std::regex_replace(line, std::regex(R"#(\D)#"), "");
    std::string number = "";
    number += line.front();
    number += line.back();
    sum += std::stoi(number);
  }
  std::cout << std::to_string(sum) << std::endl;
  writeToFile(std::to_string(sum), "day01/output1.txt");
}

bool findFirstNumberStr(
    const std::unordered_map<std::string, std::string> numbers, int start,
    std::string str, std::string &output) {
  if (start == str.length())
    return false;

  std::string word = "";
  for (int i = start; i < str.length(); i++) {
    word += str[i];
    if (numbers.find(word) != numbers.end()) {
      output = word;
      return true;
    }
  }
  return findFirstNumberStr(numbers, start + 1, str, output);
}

void day01Part2() {
  std::unordered_map<std::string, std::string> numbers{
      {"one", "1"},   {"two", "2"},   {"three", "3"},
      {"four", "4"},  {"five", "5"},  {"six", "6"},
      {"seven", "7"}, {"eight", "8"}, {"nine", "9"}};

  int sum = 0;
  for (auto line : getLinesFromFile("day01/part2.txt")) {
    std::string firstDigit = "";
    std::string secondDigit = "";
    std::string word = "";
    for (int i = 0; i < line.length(); i++) {
      word += line[i];
      if (isdigit(line[i])) {
        firstDigit = line[i];
        break;
      }
      std::string outResult;
      if (findFirstNumberStr(numbers, 0, word, outResult)) {
        firstDigit = numbers[outResult];
        break;
      }
    }
    word = "";
    for (int i = line.length() - 1; i >= 0; i--) {
      word = line[i] + word;
      if (isdigit(line[i])) {
        secondDigit = line[i];
        break;
      }
      std::string outResult;
      if (findFirstNumberStr(numbers, 0, word, outResult)) {
        secondDigit = numbers[outResult];
        break;
      }
    }

    std::string number = firstDigit + "" + secondDigit;
    sum += std::stoi(number);

    firstDigit = "";
    secondDigit = "";
  }
  std::cout << std::to_string(sum) << std::endl;
  writeToFile(std::to_string(sum), "day01/output2.txt");
}