#include "day03.hpp"
#include <iostream>
#include <regex>

struct Range {
    int start;
    int end; // exclusive

    bool contains(int val) {
        return val >= start && val < end;
    }
};

int day03_part2(const std::string &input) {
    // Had to insert another "\" before each "\" to escape it for C++
    std::regex mul_regex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    // NEW: finds all "do()"s
    std::regex do_regex("do\\(\\)");
    // NEW: finds all "don't()"s
    std::regex dont_regex("don't\\(\\)");

    // As defined in the puzzle text: Each input starts with an implicit "do()"
    std::vector<int> do_positions = {0};
    // Iterate all do_regex matches
    for (std::sregex_iterator i(input.begin(), input.end(), do_regex); i != std::sregex_iterator();
         ++i) {
        do_positions.push_back((*i).position());
    }
    std::vector<int> dont_positions;
    // Iterate all do_regex matches
    for (std::sregex_iterator i(input.begin(), input.end(), dont_regex);
         i != std::sregex_iterator(); ++i) {
        dont_positions.push_back((*i).position());
    }

    // Example #1:
    // do_positions = {0, 5, 10, 20};
    // dont_positions = {15, 17};
    // here, the first Range would be 0-15 (do()'s at 5 and 10 are ignored)
    // second range would be 20-end (dont() at 17 is ignored)

    // Example #2:
    // do_positions = {0, 5, 10, 20, 22};
    // dont_positions = {15, 17, 25};
    // here, the first Range would be 0-15 (do()'s at 5 and 10 are ignored)
    // second range would be 20-25 (do() at 22 and dont() at 17 are ignored)

    std::vector<Range> enabled_ranges;
    std::vector<int>::iterator do_iter = do_positions.begin();
    std::vector<int>::iterator do_iter_end = do_positions.end();
    std::vector<int>::iterator dont_iter = dont_positions.begin();
    std::vector<int>::iterator dont_iter_end = dont_positions.end();
    int cur_do_pos = *do_iter;
    do_iter++;
    int cur_dont_pos = *dont_iter;
    dont_iter++;
    // (using designated initializers for the struct here)
    // The first range will always be from the start to the first dont
    enabled_ranges.push_back(Range{.start = cur_do_pos, .end = cur_dont_pos});

    // Now, gradually advance the do and dont iterators making sure to skip
    // multiple do() or dont() instructions and construct the vector of Range's
    // where mul()'s are actually enabled
    int next_range_start = cur_do_pos;
    int next_range_end = cur_dont_pos;
    // Loop until either:
    // - the range ends at input.length() (meaning last instruction was a do())
    // OR
    // - the range ends at a dont() pos and there are no more do() instructions
    //   (meaning last instruction was a dont())
    while ((size_t)next_range_end < input.length() &&
           !(cur_dont_pos > cur_do_pos && do_iter == do_iter_end)) {
        // while there is do()'s before the dont(), skip those, as enabling
        // mul's while already enabled does nothing
        while (cur_do_pos < cur_dont_pos && do_iter != do_iter_end) {
            // Grab the next do() after the start
            cur_do_pos = *do_iter;
            do_iter++;
        }
        // this means that there werent enough do()'s and there will not be
        // another enabled range (since there is no do() after the next dont())
        if (cur_do_pos < cur_dont_pos) {
            break;
        }
        // at this point, cur_do_pos points to a do(), that is after the current
        // dont(). Meaning that do(), will start the next enabled Range.
        next_range_start = cur_do_pos;
        // But until that do(), there could be many dont()'s but we can skip
        // those as well, as disabling mul's while already disabled does nothing
        while (cur_dont_pos < cur_do_pos && dont_iter != dont_iter_end) {
            cur_dont_pos = *dont_iter;
            dont_iter++;
        }

        // this means there werent enough dont()'s and the last instruction in
        // the input was a do(). So we end the current range at the end of the
        // string.
        if (cur_dont_pos < cur_do_pos) {
            next_range_end = input.length();
        } else {
            // Now, the cur_dont_pos points to a dont() that is for sure behind
            // the cur_do_pos, meaning it will be the end of our current enabled
            // range
            next_range_end = cur_dont_pos;
        }
        // (using designated initializers for the struct here)
        enabled_ranges.push_back(Range{.start = next_range_start, .end = next_range_end});
    }

    int total_result = 0;
    // Iterate all mul_regex matches
    for (std::sregex_iterator i(input.begin(), input.end(), mul_regex); i != std::sregex_iterator();
         ++i) {
        std::smatch match = *i;
        int mul_position = match.position();

        for (Range &r : enabled_ranges) {
            if (r.contains(mul_position)) {
                std::sub_match num1_submatch = match[1];
                std::sub_match num2_submatch = match[2];
                int num1 = std::stoi(num1_submatch.str());
                int num2 = std::stoi(num2_submatch.str());
                total_result += num1 * num2;
                break;
            }
        }
    }

    return total_result;
};
