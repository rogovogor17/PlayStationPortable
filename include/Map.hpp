#pragma once 

#include <stdint.h>

constexpr int TILE_SIZE = 20; 
constexpr int MAP_WIDTH = 24;
constexpr int MAP_HEIGHT = 16;

enum TILE_TYPE {
    BLACK       = 0,
    GRASS       = 1, 
    BRICKS_WALL = 2,
    SPECIAL     = 3,
};

extern uint8_t game_map[MAP_HEIGHT][MAP_WIDTH];