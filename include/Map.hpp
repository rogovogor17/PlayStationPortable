#pragma once 

#include <stdint.h>
#include <set>
#include <vector>
#include "board_properties.hpp"

constexpr int TILE_SIZE  = 20; 
constexpr int MAP_WIDTH  = 24;
constexpr int MAP_HEIGHT = 16;

enum class BotType;

enum TILE_TYPE {
    BLACK       = 0,
    GRASS       = 1, 
    BRICKS_WALL = 2,
    BEDROCK     = 3,
    SPECIAL     = 4,
    SPAWN_POINTS= 5,
    SPAWN_P1    = 6,
};

struct spawnPlace {
    int x, y;
    std::set<BotType> botsFlow;
};

struct Level {
    uint8_t (*map)[MAP_WIDTH];
    spawnPlace spawn_place_p1;
    std::vector<spawnPlace> bot_spawns; // x, y
    uint16_t time_limit_ms; 
};

extern uint8_t map_level_1[MAP_HEIGHT][MAP_WIDTH];
extern uint8_t map_level_2[MAP_HEIGHT][MAP_WIDTH];
extern uint8_t map_level_3[MAP_HEIGHT][MAP_WIDTH];