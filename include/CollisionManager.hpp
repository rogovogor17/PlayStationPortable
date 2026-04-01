#pragma once
#include <vector>
#include <algorithm>
#include "Entity.hpp"

class CollisionManager {
    std::vector<std::weak_ptr<Entity>> objects_;
    std::function<void(int tileX, int tileY)> wall_destroyed_callback_;
public:
    void register_object(std::shared_ptr<Entity> obj) { objects_.push_back(obj); }
    void clear() { objects_.clear(); }

    void set_wall_destroyed_callback(std::function<void(int, int)> callback) {
        wall_destroyed_callback_ = std::move(callback);
    }
    
    bool check_collisions(std::shared_ptr<Entity> mover, Rect future_rect) {
        bool has_collision = false;
        for (auto& weak_target : objects_) {
            auto target = weak_target.lock();
            if (target == mover || !target->is_active() || target->get_owner() == mover) continue;

            Rect r = target->get_collision_rect();
            if (future_rect.x < r.x + r.w && future_rect.x + future_rect.w > r.x &&
                future_rect.y < r.y + r.h && future_rect.y + future_rect.h > r.y) {
                
                mover->on_collision(target);
                target->on_collision(mover);
                has_collision = true;
            }
        }

        if (check_map_collision(future_rect, mover->get_type())) {
            auto tmp = std::make_shared<MapWallEntity>();
            mover->on_collision(tmp);
            has_collision = true;
        }

        return has_collision;
    }
    
    void cleanup() {
        objects_.erase(
            std::remove_if(objects_.begin(), objects_.end(),
                [](const auto& weak_obj) { 
                    auto shared_obj = weak_obj.lock();
                    return !shared_obj || !shared_obj->is_active();
                }),
            objects_.end()
        );
    }

    bool check_map_collision(Rect rect, CollidableType type) {
        int start_col = rect.x / TILE_SIZE;
        int end_col   = (rect.x + rect.w - 1) / TILE_SIZE;
        int start_row = rect.y / TILE_SIZE;
        int end_row   = (rect.y + rect.h - 1) / TILE_SIZE;

        if (start_col < 0 || end_col >= MAP_WIDTH || start_row < 0 || end_row >= MAP_HEIGHT) {
            return true; 
        }

        for (int i = start_row; i <= end_row; i++) {
            for (int j = start_col; j <= end_col; j++) {
                if (game_map[i][j] == BRICKS_WALL) {
                    if (type == CollidableType::BULLET) {
                        game_map[i][j] = BLACK;
                        if (wall_destroyed_callback_) {
                            wall_destroyed_callback_(j * TILE_SIZE, i * TILE_SIZE);
                        }
                    }
                        
                    return true; 
                }
            }
        }
        return false;
    }
};
