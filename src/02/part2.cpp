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

// NEW: skip_idx for skipping the element at that index
bool is_sequence_safe_dampened(std::vector<int> &seq, size_t skip_idx) {
    bool seq_safe = true;
    SequenceState seq_state = SequenceState::UNSET;

    for (size_t i = 0; i < seq.size() - 1; i++) {
        // NEW: if i is the index to skip, skip this iteration
        if (i == skip_idx) {
            continue;
        }
        int current_num = seq[i];
        int next_num = seq[i + 1];
        // NEW: if i+1 is the index to skip, set next_num to the element after
        // skip_idx
        if (i + 1 == skip_idx) {
            // Make sure we stay inside the bounds of the vector
            if (i + 2 < seq.size()) {
                next_num = seq[i + 2];
            } else {
                break;
            }
        }

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

int day02_part2(const std::string &input) {
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

        // NEW: check if the sequence would be safe by removing one of its
        // elements (trying out all slots)
        bool seq_safe = false;
        for (size_t i = 0; i <= seq.size(); i++) {
            bool safe_without_i = is_sequence_safe_dampened(seq, i);
            seq_safe |= safe_without_i;
            // Once its safe by removing one element, it can never be unsafe
            // again
            if (seq_safe) {
                break;
            }
        }

        if (seq_safe) {
            num_safe_sequences++;
        }
    }
    return num_safe_sequences;
};
