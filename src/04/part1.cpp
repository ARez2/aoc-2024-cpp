#include "day04.hpp"
#define DEBUG
#include "utils.hpp"

/// @brief Non-template variant of `find_num_word` which calls all the templated
/// functions
/// @param lines The string matrix
/// @param word The word to find
/// @return Number of times the word was found.
int find_num_word_all(const std::vector<std::string> &lines, std::string word,
                      std::vector<std::string> &dbg_lines) {
    std::vector<SearchStatus> results;
    return find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::ZERO>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::ZERO, ReadDir::NEGATIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::POSITIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines, results);
}

int day04_part1(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;
    std::vector<std::string> lines;
    std::vector<std::string> dbg_lines;

    while (std::getline(input_stream, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);
        lines.push_back(line);
        dbg_lines.push_back(std::string(line.size(), '.'));
    }
    int total_num_xmas = find_num_word_all(lines, "XMAS", dbg_lines);

    DBG for (std::string line : dbg_lines) {
        std::cout << line << "\n";
    }
    return total_num_xmas;
};
