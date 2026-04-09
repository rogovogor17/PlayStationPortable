#pragma once
#include <vector>
#include <cstdint>
#include "Map.hpp"


class LevelManager {
private:
    std::vector<Level> levels_;
    size_t current_level_ = 0;
    
    void scan_spawn_points(Level& level) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                uint8_t tile = level.map[y][x];
                
                if (tile == SPAWN_P1)     {
                    level.spawn_place_p1 = {x * TILE_SIZE, y * TILE_SIZE};
                } 

                else if (tile == SPAWN_POINTS) {
                    level.bot_spawns.push_back({x * TILE_SIZE, y * TILE_SIZE});
                }
            }
        }
    }
    
public:
    // Конструктор, который принимает массив уровней и сканирует spawn points
    LevelManager(Level levels[], size_t num_levels) {
        for (size_t i = 0; i < num_levels; ++i) {
            Level level = levels[i];
            scan_spawn_points(level);  // Сканируем spawn points на карте
            levels_.push_back(level);
        }
    }

    void update() {
        
    }
    
    Level* get_current_level() noexcept{
        return &levels_[current_level_];
    }
    
    bool load_level(size_t level_index) noexcept {
        if (level_index >= levels_.size()) return false;
        current_level_ = level_index;
        return true;
    }
    
    bool next_level() noexcept {
        if (current_level_ + 1 >= levels_.size()) return false;
        current_level_++;
        return true;
    }
    
    bool is_last_level() const noexcept {
        return current_level_ + 1 >= levels_.size();
    }
    
    size_t get_level_number() const noexcept {
        return current_level_ + 1;
    }
    
    size_t get_total_levels() const noexcept {
        return levels_.size();
    }
};