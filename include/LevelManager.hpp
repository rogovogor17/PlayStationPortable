#pragma once
#include <vector>
#include <cstdint>
#include "Tank.hpp"

enum class LevelAction {
    CREATE_BOT,
    NEXT_LEVEL,
    RESTART_LEVEL,
};

class LevelManager {
private:
    std::vector<Level> levels_;
    size_t current_level_ = 0;

    bool level_started_    = true;
    uint32_t current_tick_ = 0;

    std::optional<LevelAction> action_;
    int last_spawn_x_ = 0, last_spawn_y_ = 0;
    BotType last_spawn_bot_type_ = BotType::normal;
    
    void scan_spawn_points(Level& level) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                uint8_t tile = level.map[y][x];
                
                if (tile == SPAWN_P1) {
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

        level_started_ = true;
    }

    void update() {
        
        if (!level_started_) {
            action_ = std::nullopt;
            return;
        }
        
        current_tick_++;
        action_ = std::nullopt;
        // Проверяем, нужно ли заспавнить ботов
        if (!is_all_bots_spawned()) {
            if (current_tick_%levels_[current_level_].time_between_spawns_tick == 0) {
                choose_location_of_spawn();
                choose_type_of_bot();
                levels_[current_level_].amount_of_bots_to_spawn--;
                action_ = LevelAction::CREATE_BOT; 
            }
        }

        // else if (is_all_bots_spawned() && tanks_.empty()) {
        //     if (!is_last_level()) {
        //         return LevelAction::NEXT_LEVEL;
        //     } else {
        //         return LevelAction::RESTART_LEVEL;
        //     }
        // }
    }

    void choose_location_of_spawn() {
        const Level& level = levels_[current_level_];
        
        if (!level.bot_spawns.empty()) {
            int random_index = rand() % level.bot_spawns.size();
            last_spawn_x_ = level.bot_spawns[random_index].x;
            last_spawn_y_ = level.bot_spawns[random_index].y;
            return;
        }
    }

    void choose_type_of_bot() {
        int random_index = rand() % 3;  // 0, 1, 2
        
        switch(random_index) {
        case 0: last_spawn_bot_type_ = BotType::easy; break;
        case 1: last_spawn_bot_type_ = BotType::normal; break;
        case 2: last_spawn_bot_type_ = BotType::hard; break;
    }
    }
    
    Level* get_current_level() noexcept{
        return &levels_[current_level_];
    }

    bool is_all_bots_spawned() const noexcept {
        return levels_[current_level_].amount_of_bots_to_spawn == 0;
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

    std::pair<int, int> get_last_spawn_location() const noexcept {
        return {last_spawn_x_, last_spawn_y_};
    }

    std::optional<LevelAction> get_action() const noexcept {
        return action_;
    }

    BotType get_last_spawn_bot_type() const noexcept {
        return last_spawn_bot_type_;
    }
};