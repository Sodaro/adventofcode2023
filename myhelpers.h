#pragma once
#include <vector>
#include <string>
#include <fstream>

void writeToFile(std::string text, std::string file)
{
    std::ofstream outputFile;
    outputFile.open(file);
    outputFile << text << std::endl;
}

std::vector<std::string> splitStrByDelim(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.emplace_back(token);
    }

    res.emplace_back(s.substr(pos_start));
    return res;
}

std::vector<std::string> getLinesFromFile(const char* path)
{
    std::ifstream myfile(path);
    std::string line;
    std::vector<std::string> lines;
    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            lines.emplace_back(line);
        }
    }
    myfile.close();
    return lines;
}
