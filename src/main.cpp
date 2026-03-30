#include "01/day01.hpp"
#include "02/day02.hpp"
#include "03/day03.hpp"
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Alias for a function that returns void and accept const string
using Solver = std::function<int(const std::string &)>;

// Maps day nr + part nr to a function so that we dont need
// an endless if/switch statement
std::map<std::pair<int, int>, Solver> solvers = {
    {{1, 1}, day01_part1}, {{1, 2}, day01_part2}, {{2, 1}, day02_part1},
    {{2, 2}, day02_part2}, {{3, 1}, day03_part1}, {{3, 2}, day03_part2},
};

std::string read_file(const std::string &path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Failed to open file");

    std::ostringstream sstr;
    sstr << file.rdbuf();
    return sstr.str();
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: ./aoc <day> <part> [--example]\n";
        return 1;
    }

    try {
        int day = std::stoi(argv[1]);
        int part = std::stoi(argv[2]);
        bool example = (argc > 3 && std::string(argv[3]) == "--example");

        std::pair<int, int> key = std::make_pair(day, part);
        if (day >= 1 && solvers.count(key) > 0) {
            std::string input_path = std::format("inputs/day{:02}{}.txt", day,
                                                 example ? "_example" : "");
            std::string input = read_file(input_path);
            int result = solvers[key](input);
            std::cout << "Puzzle output: " << result << "\n";
        } else {
            std::cerr << "Unknown day/part\n";
        }
    } catch (std::runtime_error &e) {
        std::cerr << "Error while reading file: " << e.what();
    }

    return 0;
}