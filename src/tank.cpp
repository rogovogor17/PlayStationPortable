#include "../include/tank.hpp"

void Tank::show(void) {
    tank_sprite_->pushSprite(x_pos, y_pos);
}

void Tank::move(int x, int y) {
    //printing to the screen background being strored in background_buffer_
    tft_.pushImage(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, background_buffer_); // restoring the background pixels before moving the tank to a new position

    if (x && y) return; // disallow diagonal movement

    //ensuring corresponding orientation os tank's sprite
    if (y > 0 && direction_ != TankDirection::DOWN) {
        set_direction(TankDirection::DOWN);
    } else if (y < 0 && direction_ != TankDirection::UP) {
        set_direction(TankDirection::UP);
    } else if (x < 0 && direction_ != TankDirection::LEFT) {
        set_direction(TankDirection::LEFT);
    } else if (x > 0 && direction_ != TankDirection::RIGHT) {
        set_direction(TankDirection::RIGHT);
    }

    //movement
    x_pos += x;
    y_pos += y;

    //storing background to background_buffer_
    tft_.readRect(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, background_buffer_);
}

void Tank::set_position(size_t x, size_t y) {
    x_pos = x;
    y_pos = y;
}

void Tank::set_direction(enum TankDirection direction) {
    if (direction == direction_) {
        return;
    }

    switch (direction) {
        case TankDirection::UP:
            tank_sprite_->pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank_up);
            break;
        case TankDirection::DOWN:
            tank_sprite_->pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank_down);
            break;
        case TankDirection::LEFT:
            tank_sprite_->pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank_left);  
            break;
        case TankDirection::RIGHT:
            tank_sprite_->pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank_right); 
            break;
    }
   
    direction_ = direction;
}

size_t Tank::get_x_pos() const {
    return x_pos;
}

size_t Tank::get_y_pos() const {
    return y_pos;
}

size_t Tank::get_speed() const {
    return speed_;
}

size_t Tank::get_health() const {
    return health_;
}

size_t Tank::get_max_health() const {
    return max_health_;
}

size_t Tank::get_ammunition() const {
    return ammunition_;
}

size_t Tank::get_max_ammunition() const {
    return max_ammunition_;
}