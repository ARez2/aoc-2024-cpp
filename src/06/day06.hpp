#pragma once
#include <iostream>
#include <string>
#include <vector>

int day06_part1(const std::string &);
int day06_part2(const std::string &);

/// @brief  Single coordinate in the map
typedef std::pair<int, int> Coord;

class Map {
  public:
    enum class TileType { EMPTY, OBSTACLE };
    struct Tile {
        TileType type = TileType::EMPTY;
        bool marked;
    };

    int width, height;

    Map(int w, int h) : width(w), height(h), tiles(w * h) {
    }

    void set_tile(Coord coord, TileType type) {
        tiles[coord.first * width + coord.second] = Tile{.type = type, .marked = false};
    }

    bool tile_marked(Coord coord) {
        return tiles[coord.first * width + coord.second].marked;
    }

    void mark_tile(Coord coord) {
        tiles[coord.first * width + coord.second].marked = true;
    }

    void print(Coord guard_pos = {-1, -1}) {
        char empty_char = '.';
        char obstacle_char = '#';
        char guard_char = '^';

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Tile &tile = tiles[x * width + y];
                char c;
                switch (tile.type) {
                case TileType::EMPTY:
                    c = empty_char;
                    break;
                case TileType::OBSTACLE:
                    c = obstacle_char;
                    break;
                }
                if (Coord(x, y) == guard_pos) {
                    c = guard_char;
                }
                std::cout << c;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

  private:
    std::vector<Tile> tiles;
};