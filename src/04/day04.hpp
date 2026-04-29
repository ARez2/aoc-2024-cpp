#pragma once
#include <algorithm>
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int day04_part1(const std::string &);
int day04_part2(const std::string &);

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
void search(const std::vector<std::string> &lines, Coord coord, SearchStatus &status);

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
