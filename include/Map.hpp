#pragma once 

#include <stdint.h>

constexpr int TILE_SIZE = 40; 
constexpr int MAP_WIDTH = 12;
constexpr int MAP_HEIGHT = 8;

enum TILE_TYPE {
    BLACK = 0,
    GRASS = 1, 
    BRICKS_WALL = 2,
    SPECIAL = 3,
};

constexpr uint8_t game_map[MAP_HEIGHT][MAP_WIDTH] = {
    // 0: BLACK, 1: GRASS, 2: BRICKS_WALL, 3: SPECIAL
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 0, 2, 2, 2, 0, 3, 0, 0, 1},
    {1, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 1},
    {1, 0, 3, 0, 2, 0, 2, 0, 3, 0, 0, 1},
    {1, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};