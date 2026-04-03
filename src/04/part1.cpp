#include "day04.hpp"
#define DEBUG
#include "utils.hpp"

#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// @brief  Direction for how to read inside the "matrix". For horizontal,
/// positive is to the right. For vertical, positive is down.
enum class ReadDir { NEGATIVE, ZERO, POSITIVE };

/// @brief  Single coordinate in the matrix of strings.
typedef std::pair<int, int> Coord;

/// @brief Holds the current status while searching for a string in the matrix.
/// Gets updated by `search()`
struct SearchStatus {
    std::string word_to_find;
    uint num_chars_found;
    Coord start;
    Coord end;
    ReadDir horizontal_dir;
    ReadDir vertical_dir;

    SearchStatus(std::string word, ReadDir hor_dir, ReadDir ver_dir) {
        word_to_find = word;
        num_chars_found = 0;
        start = Coord(-1, -1);
        end = Coord(-1, -1);
        horizontal_dir = hor_dir;
        vertical_dir = ver_dir;
    }

    /// @brief Returns true if the word has been found.
    bool is_complete() {
        return num_chars_found == word_to_find.length();
    }

    /// @brief  Writes the (found) `word_to_find` into `matrix` where it was found.
    /// @param matrix The string matrix to write into
    void mark_work_in_matrix(std::vector<std::string> &matrix) {
        if (!is_complete()) {
            return;
        }
        uint char_idx = 0;

        int dx = (end.first > start.first) ? 1 : (end.first < start.first) ? -1 : 0;
        int dy = (end.second > start.second) ? 1 : (end.second < start.second) ? -1 : 0;
        int x = start.first;
        int y = start.second;
        while (true) {
            matrix[y][x] = word_to_find[char_idx];
            char_idx++;
            if (x == end.first && y == end.second)
                break;
            x += dx;
            y += dy;
        }
    }
};

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
                  std::vector<std::string> &dbg_lines);

template <>
int find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines) {
    int num_found = 0;
    for (int y = 0; y < lines.size(); y++) {
        SearchStatus status = SearchStatus(word, ReadDir::POSITIVE, ReadDir::ZERO);
        for (int x = 0; x < lines[y].length(); x++) {
            search(lines, Coord(x, y), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
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
                                                    std::vector<std::string> &dbg_lines) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(lines, word, dbg_lines);
}
template <>
int find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                    std::string word,
                                                    std::vector<std::string> &dbg_lines) {
    int num_found = 0;
    // ASSUMPTION: Each line has the same length
    for (int x = 0; x < lines[0].length(); x++) {
        SearchStatus status = SearchStatus(word, ReadDir::ZERO, ReadDir::POSITIVE);
        for (int y = 0; y < lines.size(); y++) {
            search(lines, Coord(x, y), status);
            if (status.is_complete()) {
                num_found++;
                DBG status.mark_work_in_matrix(dbg_lines);
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
                                                    std::vector<std::string> &dbg_lines) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(lines, word, dbg_lines);
}
template <>
int find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines) {

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
                                                        std::vector<std::string> &dbg_lines) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(lines, word, dbg_lines);
}

template <>
int find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(const std::vector<std::string> &lines,
                                                        std::string word,
                                                        std::vector<std::string> &dbg_lines) {

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
                                                        std::vector<std::string> &dbg_lines) {
    std::reverse(word.begin(), word.end());
    return find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(lines, word, dbg_lines);
}

/// @brief Non-template variant of `find_num_word` which calls all the templated
/// functions
/// @param lines The string matrix
/// @param word The word to find
/// @return Number of times the word was found.
int find_num_word(const std::vector<std::string> &lines, std::string word,
                  std::vector<std::string> &dbg_lines) {
    // return find_num_word<ReadDir::ZERO, ReadDir::NEGATIVE>(lines, word, dbg_lines);
    return find_num_word<ReadDir::POSITIVE, ReadDir::ZERO>(lines, word, dbg_lines) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::ZERO>(lines, word, dbg_lines) +
           find_num_word<ReadDir::ZERO, ReadDir::POSITIVE>(lines, word, dbg_lines) +
           find_num_word<ReadDir::ZERO, ReadDir::NEGATIVE>(lines, word, dbg_lines) +
           find_num_word<ReadDir::POSITIVE, ReadDir::POSITIVE>(lines, word, dbg_lines) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines) +
           find_num_word<ReadDir::NEGATIVE, ReadDir::POSITIVE>(lines, word, dbg_lines) +
           find_num_word<ReadDir::POSITIVE, ReadDir::NEGATIVE>(lines, word, dbg_lines);
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
    int total_num_xmas = find_num_word(lines, "XMAS", dbg_lines);

    DBG for (std::string line : dbg_lines) {
        std::cout << line << "\n";
    }
    return total_num_xmas;
};
