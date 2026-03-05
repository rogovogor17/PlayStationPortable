#pragma once
#include <vector>
#include <algorithm>
#include "Collidable.hpp"

class CollisionManager {
    std::vector<Collidable*> objects_;
    
public:
    void register_object(Collidable* obj) { objects_.push_back(obj); }
    void clear() { objects_.clear(); }
    
    bool check_collisions(Collidable* mover, Rect future_rect) {
        bool has_collision = false;
        for (auto* target : objects_) {
            if (target == mover || !target->is_active()) continue;

            Rect r = target->get_collision_rect();
            // Проверка пересечения Rect
            if (future_rect.x < r.x + r.w && future_rect.x + future_rect.w > r.x &&
                future_rect.y < r.y + r.h && future_rect.y + future_rect.h > r.y) {
                
                // Уведомляем оба объекта!
                mover->on_collision(target);
                target->on_collision(mover);
                has_collision = true;
            }
        }
        return has_collision;
    }
    
    void cleanup() {
        objects_.erase(
            std::remove_if(objects_.begin(), objects_.end(),
                [](const auto& obj) { return !obj->is_active(); }),
            objects_.end()
        );
    }
};
