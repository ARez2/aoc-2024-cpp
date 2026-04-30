#include "day06.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "term_util.hpp"

int day06_part1(const std::string &input) {
    std::istringstream input_stream(input);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(input_stream, line)) {
        lines.push_back(line);
    }

    int width = lines[0].length();
    int height = lines.size();

    Map map = Map(width, height);

    Coord guard_pos = {-1, -1};

    for (int y = 0; y < lines.size(); y++) {
        for (int x = 0; x < lines[y].length(); x++) {
            char c = lines[y][x];
            Coord coord = {x, y};
            switch (c) {
            case '.':
                map.set_tile(coord, Map::TileType::EMPTY);
                break;
            case '#':
                map.set_tile(coord, Map::TileType::OBSTACLE);
                break;
            case '^':
                guard_pos = coord;
                map.set_tile(coord, Map::TileType::EMPTY);
                break;
            }
        }
    }

    map.print(guard_pos);

    return 0;
};
