#pragma once
#include "../myhelpers.h"
#include <cstdint>
#include <regex>
#include <utility>

struct Data {
  std::string name = "";
  std::pair<uint64_t, uint64_t> destinationRange;
  std::pair<uint64_t, uint64_t> sourceRange;
};

struct DataGroup {
  std::vector<Data *> dataEntries;
  uint64_t getDestinationFromSource(uint64_t source, Data *outDataPtr) {
    for (auto data : dataEntries) {
      if (source <= data->sourceRange.second &&
          source >= data->sourceRange.first) {
        outDataPtr = data;
        auto offset = data->sourceRange.second - source;
        return data->destinationRange.second - offset;
      }
    }
    // if not mapped, return same value
    outDataPtr = nullptr;
    return source;
  }

  uint64_t getDestinationFromSource(uint64_t source) {
    for (auto data : dataEntries) {
      if (source <= data->sourceRange.second &&
          source >= data->sourceRange.first) {
        auto offset = data->sourceRange.second - source;
        return data->destinationRange.second - offset;
      }
    }
    // if not mapped, return same value
    return source;
  }

  Data *getClosestDataToSource(uint64_t source) {
    uint64_t closestDistance = UINT64_MAX;
    Data *closestData = dataEntries[0];
    for (auto data : dataEntries) {
      uint64_t dataDistance = UINT64_MAX;
      if (source < data->sourceRange.first) {
        dataDistance = data->sourceRange.first - source;
      } else if (source > data->sourceRange.second)
        dataDistance = source - data->sourceRange.second;

      if (dataDistance < closestDistance) {
        closestDistance = dataDistance;
        closestData = data;
      }
    }
    return closestData;
  }
  // uint64_t getSourceFromDestination(uint64_t destination) {
  //   for (auto data : dataEntries) {
  //     if (destination <= data->destinationRange.second &&
  //         destination >= data->destinationRange.first) {
  //       auto offset = data->destinationRange.second - destination;
  //       return data->sourceRange.second - offset;
  //     }
  //   }
  //   // if not mapped, return same value
  //   return destination;
  // }
};

void day05() {
  // first: seed | second: soil
  // part1: get lowest location number from all seeds
  DataGroup soil;
  DataGroup fertilizer;
  DataGroup water;
  DataGroup light;
  DataGroup temperature;
  DataGroup humidity;
  DataGroup location;

  auto lines = aoc::getLinesFromFile("day05/test.txt");

  std::string currentCategory = "";
  DataGroup *targetDataGroup = &soil;
  for (int i = 2; i < lines.size(); i++) {
    if (isalpha(lines[i][0])) {
      std::smatch sm;
      std::regex_search(lines[i], sm, std::regex("-(\\w+) "));
      currentCategory = sm[1];
      if (currentCategory == "soil") {
      } else if (currentCategory == "fertilizer") {
        targetDataGroup = &fertilizer;
      } else if (currentCategory == "water") {
        targetDataGroup = &water;
      } else if (currentCategory == "light") {
        targetDataGroup = &light;
      } else if (currentCategory == "temperature") {
        targetDataGroup = &temperature;
      } else if (currentCategory == "humidity") {
        targetDataGroup = &humidity;
      } else if (currentCategory == "location") {
        targetDataGroup = &location;
      }
    } else if (isdigit(lines[i][0])) {
      auto values = aoc::splitStrByDelim(lines[i], " ");

      Data *newData = targetDataGroup->dataEntries.emplace_back(new Data());
      auto destinationRangeStart = std::stoul(values[0]);
      auto sourceRangeStart = std::stoul(values[1]);
      auto length = std::stoul(values[2]);
      auto destinationRange = std::make_pair<uint64_t, uint64_t>(
          destinationRangeStart, destinationRangeStart + length - 1);
      auto sourceRange = std::make_pair<uint64_t, uint64_t>(
          sourceRangeStart, sourceRangeStart + length - 1);
      newData->name = currentCategory;
      newData->destinationRange = destinationRange;
      newData->sourceRange = sourceRange;
    }
  }
  std::string seedsInput = lines[0];
  std::string seedValues = aoc::splitStrByDelim(seedsInput, "seeds: ")[0];

  uint64_t part1lowestLocationValue = UINT64_MAX;
  for (auto seed : aoc::splitStrByDelim(seedValues, " ")) {
    uint64_t seedValue = std::stoul(seed);
    uint64_t soilValue = soil.getDestinationFromSource(seedValue);
    uint64_t fertilizerValue = fertilizer.getDestinationFromSource(soilValue);
    uint64_t waterValue = water.getDestinationFromSource(fertilizerValue);
    uint64_t lightValue = light.getDestinationFromSource(waterValue);
    uint64_t temperatureValue =
        temperature.getDestinationFromSource(lightValue);
    uint64_t humidityValue =
        humidity.getDestinationFromSource(temperatureValue);
    uint64_t locationValue = location.getDestinationFromSource(humidityValue);
    part1lowestLocationValue =
        std::min(part1lowestLocationValue, locationValue);
  }

  std::cout << part1lowestLocationValue << std::endl;

  uint64_t part2lowestLocationValue = UINT64_MAX;
  auto part2Input = aoc::splitStrByDelim(seedValues, " ");

  uint64_t seedUpperBound = UINT64_MAX;
  uint64_t seedLowerBound = 0;

  //522761994 is too high

  for (int i = 0; i < part2Input.size() - 1; i += 2) {
    auto seed = std::stoul(part2Input[i]);
    auto range = std::stoul(part2Input[i + 1]);
    for (uint64_t j = 0; j < range; j++) {
      if (seed + j < seedLowerBound) {
        if (seedLowerBound > seed + range) {
          continue;
        } else {
          j = seedLowerBound - seed;
        }
      } else if (seed + j > seedUpperBound) {
        continue;
      }
      Data *soilDataPtr = nullptr;
      Data *locationDataPtr = nullptr;
      uint64_t soilValue = soil.getDestinationFromSource(seed + j, soilDataPtr);
      uint64_t fertilizerValue = fertilizer.getDestinationFromSource(soilValue);
      uint64_t waterValue = water.getDestinationFromSource(fertilizerValue);
      uint64_t lightValue = light.getDestinationFromSource(waterValue);
      uint64_t temperatureValue =
          temperature.getDestinationFromSource(lightValue);
      uint64_t humidityValue =
          humidity.getDestinationFromSource(temperatureValue);
      uint64_t locationValue =
          location.getDestinationFromSource(humidityValue, locationDataPtr);
      part2lowestLocationValue =
          std::min(part2lowestLocationValue, locationValue);

      if (soilDataPtr != nullptr)
        j += soilDataPtr->sourceRange.second;
      else {
        Data *closestSoilData = soil.getClosestDataToSource(seed + j);
        if (closestSoilData != nullptr) {
          if (seed + j > closestSoilData->sourceRange.second) {
            seedUpperBound = closestSoilData->sourceRange.second;
          } else if (seed + j < closestSoilData->sourceRange.first)
            seedLowerBound = closestSoilData->sourceRange.first;
        }
      }
    }
  }

  std::cout << part2lowestLocationValue << std::endl;
}