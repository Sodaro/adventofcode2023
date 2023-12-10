#pragma once
#include "../myhelpers.h"
void day06() {
  // totaltime = chargetime + traveltime
  // longer chargeduration = faster speed
  // can only charge at start of race
  // +1ms chargetime = +1mm/ms speed
  // output = nr of ways to beat each race multiplied together
  auto lines = aoc::getLinesFromFile("day06/input.txt");
  std::string timeInput = aoc::splitStrByDelim(lines[0], "Time:")[0];
  std::string distanceInput = aoc::splitStrByDelim(lines[1], "Distance:")[0];
  std::vector<std::string> timeValues = aoc::splitStrByDelim(timeInput, " ");
  std::vector<std::string> distanceValues =
      aoc::splitStrByDelim(distanceInput, " ");
  std::vector<int> nrOfWaysToBeatRaces(timeValues.size(), 0);

  // t == s
  // s*(T-t) == t*(T-t)
  // t*(T-t) >= d
  //
  // d = 9, T = 7
  // >= 1 as we can't move for 0 duration
  // -t^2+t*7+(-9) >= 1
  // -t^2+7t+(-10) >= 0
  // roots are our min and max times

  for (int i = 0; i < timeValues.size(); i++) {
    int raceDuration = std::stoi(timeValues[i]);
    int distanceToBeat = std::stoi(distanceValues[i]);
    int t = raceDuration;
    int d = -(distanceToBeat + 1);
    float root1 = (-1 * t - std::sqrtf(std::pow(t, 2) - (-4 * d))) / -2;
    float root2 = (-1 * t + std::sqrtf(std::pow(t, 2) - (-4 * d))) / -2;
    nrOfWaysToBeatRaces[i] = (int)(std::floor(root1) - std::floor(root2));
  }
  int part1Output = 1;
  std::for_each(nrOfWaysToBeatRaces.begin(), nrOfWaysToBeatRaces.end(),
                [&part1Output](int val) { part1Output *= val; });
  std::cout << part1Output << std::endl;

  std::string part2TimeInput =
      std::accumulate(timeValues.begin(), timeValues.end(), std::string{});

  std::string part2DistanceInput = std::accumulate(
      distanceValues.begin(), distanceValues.end(), std::string{});

  int64_t raceDuration = std::stoll(part2TimeInput);
  int64_t distanceToBeat = std::stoll(part2DistanceInput);

  int64_t t = raceDuration;
  int64_t d = -(distanceToBeat + 1);
  float root1 = (-1 * t - std::sqrtf(std::pow(t, 2) - (-4 * d))) / -2;
  float root2 = (-1 * t + std::sqrtf(std::pow(t, 2) - (-4 * d))) / -2;
  int64_t part2Output = (int64_t)(std::floor(root1) - std::floor(root2));

  std::cout << part2Output << std::endl;
  aoc::writeToFile(std::to_string(part1Output), "day06/part1.txt");
  aoc::writeToFile(std::to_string(part2Output), "day06/part2.txt");
}