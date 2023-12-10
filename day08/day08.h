#pragma once
#include "../myhelpers.h"
#include <regex>
#include <unordered_set>
struct Node {
  std::string name;
  std::string left;
  std::string right;
};

void day08() {
  auto lines = aoc::getLinesFromFile("day08/input.txt");
  std::string moveInstructions = lines[0];

  std::unordered_map<std::string, Node *> nodes;
  std::vector<Node *> nodesThatEndWithA;
  std::unordered_set<std::string> nodesThatEndWithZNames;
  for (int i = 2; i < lines.size(); i++) {
    std::vector<std::string> nodeInput = aoc::splitStrByDelim(lines[i], " = ");
    std::string name = nodeInput[0];

    nodes[name] = new Node();
    nodes[name]->name = name;

    if (name.ends_with('A'))
      nodesThatEndWithA.push_back(nodes[name]);
    else if (name.ends_with('Z'))
      nodesThatEndWithZNames.insert(name);

    std::smatch sm;
    std::regex_search(nodeInput[1], sm, std::regex("\\w+, \\w+"));
    std::vector<std::string> otherNodesInput =
        aoc::splitStrByDelim(sm[0], ", ");
    nodes[name]->left = otherNodesInput[0];
    nodes[name]->right = otherNodesInput[1];
  }

  uint32_t instructionIndex = 0;
  Node *currentNode = nodes["AAA"];
  //uint32_t numberOfStepsToReachZ = 0;
  //while (currentNode->name != "ZZZ") {
  //  char instruction = moveInstructions[instructionIndex];
  //  if (instruction == 'R')
  //    currentNode = nodes[currentNode->right];
  //  else
  //    currentNode = nodes[currentNode->left];
  //  ++instructionIndex %= moveInstructions.size();
  //  ++numberOfStepsToReachZ;
  //}
  //std::cout << numberOfStepsToReachZ << std::endl;


  auto stepsToReachEndWithZ = [&nodesThatEndWithA, &moveInstructions,
                               &nodes](Node* node) {
    uint32_t instructionIndex = 0;
    Node *currentNode = node;
    uint32_t stepsToReachEndWithZ = 0;
    while (!currentNode->name.ends_with('Z')) {
      char instruction = moveInstructions[instructionIndex];
      if (instruction == 'R')
        currentNode = nodes[currentNode->right];
      else
        currentNode = nodes[currentNode->left];
      ++instructionIndex %= moveInstructions.size();
      ++stepsToReachEndWithZ;
    }
    return stepsToReachEndWithZ;
  };


  uint64_t totalSteps = 1;
  for (auto node : nodesThatEndWithA) {
    totalSteps = std::lcm(totalSteps, stepsToReachEndWithZ(node));
  }
  //20659 not correct
  std::cout << totalSteps << std::endl;
}