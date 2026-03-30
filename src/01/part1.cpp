#include "day01.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int day01_part1(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;
    std::vector<int> left;
    std::vector<int> right;

    while (std::getline(input_stream, line)) {
        std::istringstream stream(line);
        int num1, num2;
        stream >> num1 >> num2;
        left.push_back(num1);
        right.push_back(num2);
    }

    // Sort both columns
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int total_distance = 0;
    for (size_t i = 0; i < left.size(); i++) {
        int max = std::max(left[i], right[i]);
        int min = std::min(left[i], right[i]);
        total_distance += max - min;
    }

    return total_distance;
};
