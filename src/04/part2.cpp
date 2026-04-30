#include "day04.hpp"
#define DEBUG
#include "utils.hpp"

#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

/// @brief Non-template variant of `find_num_word` which calls all the templated
/// functions but only for the diagonal directions
/// @param lines The string matrix
/// @param word The word to find
/// @return Number of times the word was found.
int find_num_word_diag(const std::vector<std::string> &lines, std::string word,
                       std::vector<std::string> &dbg_lines, std::vector<SearchStatus> &results) {
    return find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results) +
           find_num_word<ReadDir::POSITIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines, results);
}

/// @brief Make sure that each occurancy of MAS also is part of an X-MAS (so two MAS sharing an "A")
/// @param results The results from the MAS search
/// @param dbg_lines The debug visualization string matrix
/// @return A vector of MAS-findings which all belong to an X-MAS
std::vector<SearchStatus> validate_x_mas_results(std::vector<SearchStatus> &results,
                                                 std::vector<std::string> &dbg_lines) {
    std::unordered_set<size_t> verified_indices;

    for (u_int32_t i = 0; i < results.size(); i++) {
        if (verified_indices.contains(i)) {
            continue;
        }
        SearchStatus res = results[i];
        // For each result, we need to find its pair-result which forms the X-MAS, for example:
        // M.S
        // .A.
        // M.S
        // First option for the pair-result start and end
        Coord opt1_pair_start = Coord(res.end.first, res.start.second);
        Coord opt1_pair_end = Coord(res.start.first, res.end.second);
        // ReadDir opt1_pair_dir =
        //  since the pair-result can go two ways diagonally, have a second option
        Coord opt2_pair_start = opt1_pair_end;
        Coord opt2_pair_end = opt1_pair_start;
        for (u_int32_t j = 0; j < results.size(); j++) {
            SearchStatus other_res = results[j];
            if ((other_res.start == opt1_pair_start && other_res.end == opt1_pair_end) ||
                (other_res.start == opt2_pair_start && other_res.end == opt2_pair_end)) {
                verified_indices.insert(i);
                verified_indices.insert(j);
            }
        }
    }

    std::vector<SearchStatus> verified_results;
    for (u_int32_t verified_idx : verified_indices) {
        results[verified_idx].mark_work_in_matrix(dbg_lines);
        verified_results.push_back(results[verified_idx]);
    }
    return verified_results;
}

int day04_part2(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;
    std::vector<std::string> lines;
    std::vector<std::string> dbg_lines;

    while (std::getline(input_stream, line)) {
        line.erase(line.find_last_not_of(" \n\r\t") + 1);
        lines.push_back(line);
        dbg_lines.push_back(std::string(line.size(), '.'));
    }
    std::vector<std::string> dbg_lines2 = dbg_lines;

    std::vector<SearchStatus> results;
    int _ = find_num_word_diag(lines, "MAS", dbg_lines, results);
    std::vector<SearchStatus> verified_result = validate_x_mas_results(results, dbg_lines2);
    // Since we need to find "an X-MAS" (so 2x "MAS" together), divide the result by two
    int total_num_x_mas = verified_result.size() / 2;

    DBG for (std::string line : dbg_lines2) {
        std::cout << line << "\n";
    }
    return total_num_x_mas;
};
