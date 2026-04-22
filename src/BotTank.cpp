#include "BotTank.hpp"
#include <cmath>

void BotTank::set_type(const BotType& type) {
    type_ = type;
    switch(type) {
        case BotType::easy: {
            this->set_health(10);
            this->set_speed(3);
            break;
        }

        case BotType::normal: {
            this->set_health(30);
            this->set_speed(5);
            break;
        }   

        case BotType::hard: {
            this->set_health(50);
            this->set_speed(3);
            break;
        }

        default: break;
    }
}

void BotTank::handle_movement() {
    
    unsigned long now = millis();
    
    // Принимаем решения
    if (now - last_decision_time_ > DECISION_INTERVAL) {
        make_decision();
        last_decision_time_ = now;
    }
}

void BotTank::make_decision() {
    if (wants_to_shoot()) {
        shoot();
    }

    if (get_valid_dir_callback_) {
        Rect current_rect = get_collision_rect();
        int speed = get_speed();
        
        std::vector<Direction> valid_dirs = get_valid_dir_callback_(speed, current_rect);
        
        if (!valid_dirs.empty()) {
            int random_index = rand() % valid_dirs.size();
            setOrientation(valid_dirs[random_index]);
        }
    }
}

bool BotTank::wants_to_shoot() {
    if (!this->canShoot()) return false;
    
    unsigned long now = millis();
    if (now - last_shot_time_ < this->get_shoot_cooldown()*2) return false;
    
    // Просто стреляем с заданным интервалом, без проверки расстояния
    return true;
}