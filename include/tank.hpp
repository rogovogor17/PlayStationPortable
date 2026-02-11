#ifndef CLASS_TANK
#define CLASS_TANK

#include <iostream>
#include <TFT_eSPI.h> 
#include "./Textures/default_tank.h"

class Tank {
    const size_t max_health_, max_ammunition_; 
    size_t health_, ammunition_; 

    size_t x_pos, y_pos; // position of the tank on the board
    TFT_eSPI& tft_;
    TFT_eSprite* tank_sprite_;
    uint16_t background_buffer_[DEFAULT_TANK_WIDTH * DEFAULT_TANK_HEIGHT]; // buffer to store the background pixels before drawing the tank

    public:
        Tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, TFT_eSPI& tft) : //add try/catch module
        tft_(tft),
        x_pos(x_pos),                y_pos(y_pos),
        max_health_(health),         health_(health),
        max_ammunition_(ammunition), ammunition_(ammunition) { 
            tank_sprite_ = new TFT_eSprite(&tft);
            tank_sprite_->createSprite(DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT);
            tank_sprite_->setSwapBytes(true);
            tank_sprite_->pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank);
            tft.readRect(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, background_buffer_); // storing buffer of the background pixels before drawing the tank
        };

        ~Tank() {
            delete tank_sprite_;
        }

        void show(void);
        void move(size_t x, size_t y);

        void set_position(size_t x, size_t y);

        size_t get_x_pos() const;
        size_t get_y_pos() const;

        size_t get_health() const;
        size_t get_ammunition() const;
        size_t get_max_health() const;
        size_t get_max_ammunition() const;
};

#endif