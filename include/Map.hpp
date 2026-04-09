#pragma once 

#include <stdint.h>
#include "board_properties.hpp"
constexpr int TILE_SIZE  = 20; 
constexpr int MAP_WIDTH  = 24;
constexpr int MAP_HEIGHT = 16;

enum TILE_TYPE {
    BLACK       = 0,
    GRASS       = 1, 
    BRICKS_WALL = 2,
    BEDROCK     = 3,
    SPECIAL     = 4,
    SPAWN_POINTS= 5,
    SPAWN_P1    = 6,
};

extern uint8_t map_level_1[MAP_HEIGHT][MAP_WIDTH];