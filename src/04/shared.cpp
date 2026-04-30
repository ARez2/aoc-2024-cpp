#include "day04.hpp"
#define DEBUG
#include "utils.hpp"

#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// @brief Looks up the `coord` inside of `lines` and advances the `status` according to if it was a
/// match or not.
/// @param lines The string matrix
/// @param coord The coordinate (x,y) at which to look into the `lines` matrix
/// @param status The current search status
void search(const std::vector<std::string> &lines, Coord coord, SearchStatus &status) {
    const char &c = lines[coord.second][coord.first];
    // Ensure that the following indexing operation is inside the string
    if (status.num_chars_found < status.word_to_find.length()) {
        char &target_char = status.word_to_find[status.num_chars_found];
        if (c == target_char) {
            status.num_chars_found++;
            DBG std::cout << "Found char '" << c << "' of word '" << status.word_to_find
                          << "' at coord: (" << coord.first << ", " << coord.second << ")\n";
            if (status.num_chars_found == 1) {
                status.start = coord;
            }
            if (status.is_complete()) {
                status.end = coord;
                DBG std::cout << "Found word '" << status.word_to_find << "' starting at coord: ("
                              << status.start.first << ", " << status.start.second
                              << ") ending at: (" << status.end.first << ", " << status.end.second
                              << ")\n\n";
            }
        } else {
            bool search_was_ongoing = status.num_chars_found > 0;
            status.num_chars_found = 0;
            if (search_was_ongoing) {
                // status.start = Coord(-1, -1);
                // status.end = Coord(-1, -1);
                DBG std::cout << "Oops, char '" << c << "' did not match our target char '"
                              << target_char << "'. Stopping the count!\n\n";
                // Use the reset status to check if the word maybe starts now
                search(lines, coord, status);
            }
        }
    }
}

/// @brief  Templated function to find a word in a matrix consisting of lines of
/// strings.
/// @tparam horizontal_dir Reading direction horizontally
/// @tparam vertical_dir Reading direction vertically
/// @param lines The string matrix
/// @param word The word to search for
/// @return Number of times the word was found
template <ReadDir horizontal_dir, ReadDir vertical_dir>
int find_num_word(const std::vector<std::string> &lines, std::string word,
                  std::vector<std::string> &dbg_lines, std::vector<SearchStatus> &results);

template <>
int find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines,
                                                    std::vector<SearchStatus> &results) {
    int num_found = 0;
    for (size_t y = 0; y < lines.size(); y++) {
        SearchStatus status = SearchStatus(word, ReadDir::POSITIVE, ReadDir::ZERO);
        for (size_t x = 0; x < lines[y].length(); x++) {
            search(lines, Coord(x, y), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
                results.push_back(status);
                // Reset the status
                status = SearchStatus(word, ReadDir::POSITIVE, ReadDir::ZERO);
            }
        }
    }
    return num_found;
}
template <>
int find_num_word<ReadDir::NEGATIVE, ReadDir::ZERO>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines,
                                                    std::vector<SearchStatus> &results) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(lines, word, dbg_lines, results);
}
template <>
int find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines,
                                                    std::vector<SearchStatus> &results) {
    int num_found = 0;
    // ASSUMPTION: Each line has the same length
    for (size_t x = 0; x < lines[0].length(); x++) {
        SearchStatus status = SearchStatus(word, ReadDir::ZERO, ReadDir::POSITIVE);
        for (size_t y = 0; y < lines.size(); y++) {
            search(lines, Coord(x, y), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
                results.push_back(status);
                // Reset the status
                status = SearchStatus(word, ReadDir::ZERO, ReadDir::POSITIVE);
            }
        }
    }
    return num_found;
}
template <>
int find_num_word<ReadDir::ZERO, ReadDir::NEGATIVE>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines,
                                                    std::vector<SearchStatus> &results) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(lines, word, dbg_lines, results);
}
template <>
int find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines,
                                                        std::vector<SearchStatus> &results) {

    int num_found = 0;
    // https://stackoverflow.com/a/1779242
    int n = lines[0].length(); // ASSUMPTION: Each line has the same length
    for (int slice = 0; slice < 2 * n - 1; ++slice) {
        SearchStatus status = SearchStatus(word, ReadDir::POSITIVE, ReadDir::POSITIVE);
        int z = (slice < n) ? 0 : slice - n + 1;
        for (int j = z; j <= slice - z; ++j) {
            search(lines, Coord(j, (n - 1) - (slice - j)), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
                results.push_back(status);
                // Reset the status
                status = SearchStatus(word, ReadDir::POSITIVE, ReadDir::POSITIVE);
            }
        }
    }
    return num_found;
}
template <>
int find_num_word<ReadDir::NEGATIVE, ReadDir::NEGATIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines,
                                                        std::vector<SearchStatus> &results) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results);
}

template <>
int find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines,
                                                        std::vector<SearchStatus> &results) {

    int num_found = 0;
    // https://stackoverflow.com/a/1779242
    int n = lines[0].length(); // ASSUMPTION: Each line has the same length
    for (int slice = 0; slice < 2 * n - 1; ++slice) {
        SearchStatus status = SearchStatus(word, ReadDir::NEGATIVE, ReadDir::POSITIVE);
        int z = (slice < n) ? 0 : slice - n + 1;
        for (int j = z; j <= slice - z; ++j) {
            search(lines, Coord(j, slice - j), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
                results.push_back(status);
                // Reset the status
                status = SearchStatus(word, ReadDir::NEGATIVE, ReadDir::POSITIVE);
            }
        }
    }
    return num_found;
}
template <>
int find_num_word<ReadDir::POSITIVE, ReadDir::NEGATIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines,
                                                        std::vector<SearchStatus> &results) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(lines, word, dbg_lines, results);
}
