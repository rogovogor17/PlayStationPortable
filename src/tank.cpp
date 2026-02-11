#include "../include/tank.hpp"

void Tank::push_to_sprite(TFT_eSprite& screen_sprite) {
    tank_sprite_->pushToSprite(&screen_sprite, x_pos, y_pos);
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