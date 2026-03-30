#include "day03.hpp"
#include <iostream>
#include <regex>

int day03_part1(const std::string &input) {
    // Had to insert another "\" before each "\" to escape it for C++
    std::regex mul_regex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    std::sregex_iterator start =
        std::sregex_iterator(input.begin(), input.end(), mul_regex);
    std::sregex_iterator end = std::sregex_iterator();

    int total_result = 0;
    for (std::sregex_iterator i = start; i != end; ++i) {
        std::smatch match = *i;
        std::sub_match num1_submatch = match[1];
        std::sub_match num2_submatch = match[2];
        int num1 = std::stoi(num1_submatch.str());
        int num2 = std::stoi(num2_submatch.str());
        total_result += num1 * num2;
    }

    return total_result;
};
