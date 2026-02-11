#ifndef GAME_CLASS
#define GAME_CLASS

#include <SPI.h>
#include <TFT_eSPI.h> 
#include <vector>
#include <memory>
#include "Board_properties.hpp"
#include "tank.hpp"

const size_t COUNT_DOWN = 5;

class game final {
    TFT_eSPI& tft_;

    std::vector<std::unique_ptr<Tank>> tanks_;

    public:
        game(TFT_eSPI& tft) : tft_(tft) {};

        game& operator=(game& other) = delete; // delete copy assignment operator
        game(game& other)  = delete; // delete copy constructor
        game(game&& other) = delete; // delete move constructor

        void create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition) {
            auto tank = std::make_unique<Tank>(x_pos, y_pos, health, ammunition, tft_);
            tanks_.push_back(std::move(tank)); 
        }

        void delete_tank(size_t index) {
            tanks_.erase(tanks_.begin() + index);
        }

        void delete_enemy_tanks(void) {
            if (tanks_.size() > 1) {
                tanks_.erase(tanks_.begin() + 1, tanks_.end());
            }
        }

        void start(void);
};

void CountDown(TFT_eSPI& tft);

#endif