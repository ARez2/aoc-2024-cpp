#include "day05.hpp"
#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int day05_part2(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;

    Graph graph;
    std::vector<std::vector<int>> updates;

    u_int32_t update_nr = 0;
    while (std::getline(input_stream, line)) {
        std::istringstream stream(line);
        if (line.contains('|')) {
            int num1, num2;
            char _;
            stream >> num1 >> _ >> num2;
            graph.add_edge(num1, num2);
        } else {
            if (line.empty()) {
                continue;
            }
            // Insert a new update vector
            updates.push_back(std::vector<int>());
            int num;
            char c;
            // Fill the update vector with numbers
            while (stream >> num) {
                updates[update_nr].push_back(num);
                stream >> c;
            }
            update_nr += 1;
        }
    }

    int result = 0;
    for (std::vector<int> update : updates) {
        // NEW: NOT has path
        if (!graph.has_path(update)) {
            std::vector<Vertex> fixed_update = graph.find_path_using_vertices(update);
            graph.print_path(fixed_update);
            int middle = fixed_update[fixed_update.size() / 2];
            result += middle;
        }
    }
    return result;
};
