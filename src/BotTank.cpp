#include "BotTank.hpp"
#include <cmath>

void BotTank::set_type(const BotType& type) {
    type_ = type;
    if (type == BotType::normal) {
        this->set_speed(5);  // Нужно сделать speed_ protected или добавить setter
    } else if (type == BotType::hard) {
        this->set_health(75);
        this->set_speed(1);
    }
}

void BotTank::handle_movement() {
    if (!target_ || target_->is_dead() || is_dead() || is_exploding()) return;
    
    unsigned long now = millis();
    
    // Принимаем решения
    if (now - last_decision_time_ > DECISION_INTERVAL) {
        make_decision();
        last_decision_time_ = now;
    }
    
    // Двигаемся
    if (now - last_move_time_ > MOVE_INTERVAL) {
        chase_target();
        last_move_time_ = now;
    }
}

void BotTank::make_decision() {
    if (!target_) return;
    
    Rect my_rect = get_collision_rect();
    Rect target_rect = target_->get_collision_rect();
    int dx = target_rect.x - my_rect.x;
    int dy = target_rect.y - my_rect.y;
    int distance = std::abs(dx) + std::abs(dy);
    
    state_ = (distance < 150) ? AIState::ATTACK : AIState::CHASE;
}

void BotTank::chase_target() {
    if (!target_) return;
    
    Rect my_rect = get_collision_rect();
    Rect target_rect = target_->get_collision_rect();
    
    int dx = 0, dy = 0;
    if (target_rect.x < my_rect.x) dx = -this->get_speed();
    else if (target_rect.x > my_rect.x) dx = this->get_speed();
    
    if (target_rect.y < my_rect.y) dy = -this->get_speed();
    else if (target_rect.y > my_rect.y) dy = this->get_speed();
    
    if (dx != 0 || dy != 0) {
        update_orientation(dx, dy);
    }
}

bool BotTank::wants_to_shoot() {
    if (!target_ || target_->is_dead()) return false;
    if (millis() - last_shot_time_ < this->get_shoot_cooldown()) return false;
    if (!this->canShoot()) return false;
    
    Rect my_rect = get_collision_rect();
    Rect target_rect = target_->get_collision_rect();
    int dx = target_rect.x - my_rect.x;
    int dy = target_rect.y - my_rect.y;
    int distance = std::abs(dx) + std::abs(dy);
    
    return distance < 200;
}