#include "../include/tank.hpp"

void Tank::show(void) {
    tank_sprite_->pushSprite(x_pos, y_pos);
}

void Tank::move(size_t x, size_t y) {
    tft_.pushImage(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, background_buffer_); // restoring the background pixels before moving the tank to a new position

    x_pos += x;
    y_pos += y;

    tank_sprite_->pushSprite(x_pos, y_pos);
    tft_.readRect(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, background_buffer_);
}

void Tank::set_position(size_t x, size_t y) {
    x_pos = x;
    y_pos = y;
}

size_t Tank::get_x_pos() const {
    return x_pos;
}

size_t Tank::get_y_pos() const {
    return y_pos;
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