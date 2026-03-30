#include "day02.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

enum class SequenceState {
    UNSET,
    INCREASING,
    DECREASING,
};

bool is_sequence_safe(std::vector<int> &seq) {
    bool seq_safe = true;
    SequenceState seq_state = SequenceState::UNSET;

    for (int i = 0; i < seq.size() - 1; i++) {
        int current_num = seq[i];
        int next_num = seq[i + 1];
        int diff = next_num - current_num;
        // If its unset, set the sequence state to be either increasing or
        // decreasing
        if (seq_state == SequenceState::UNSET) {
            if (sign(diff) >= 0) {
                seq_state = SequenceState::INCREASING;
            } else if (sign(diff) < 0) {
                seq_state = SequenceState::DECREASING;
            }
        } else { // else, check if the current step from
            // current_num to next_num violates the current state

            // in this case, the sequence was decreasing but this step
            // increased.
            bool violates_decreasing_seq =
                seq_state == SequenceState::DECREASING && sign(diff) >= 0;
            // in this case, the sequence was increasing but this step
            // decreased.
            bool violates_increasing_seq =
                seq_state == SequenceState::INCREASING && sign(diff) < 0;
            if (violates_decreasing_seq || violates_increasing_seq) {
                seq_safe = false;
                break;
            }
        }
        // after this, rule 1 of the task is satisfied.

        // now check for rule 2:
        if (abs(diff) < 1 || abs(diff) > 3) {
            seq_safe = false;
            break;
        }

        current_num = next_num;
    }

    return seq_safe;
}

int day02_part1(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;

    int num_safe_sequences = 0;
    while (std::getline(input_stream, line)) {
        std::istringstream line_stream(line);
        std::vector<int> seq;
        // Assumption: all levels are positive
        int next_num = -1;
        while (line_stream >> next_num) {
            seq.push_back(next_num);
        }

        bool seq_safe = is_sequence_safe(seq);
        if (seq_safe) {
            num_safe_sequences++;
        }
    }
    return num_safe_sequences;
};
