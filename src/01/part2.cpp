#include "day01.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int day01_part2(const std::string &input) {
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

    int similarity = 0;
    for (size_t i = 0; i < left.size(); i++) {
        int num = left[i];
        int occurances_in_right = std::count(right.begin(), right.end(), num);
        similarity += num * occurances_in_right;
    }

    return similarity;
};
