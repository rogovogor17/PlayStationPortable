#ifndef GAME_CLASS
#define GAME_CLASS

#include <SPI.h>
#include <TFT_eSPI.h> 
#include <vector>
#include <memory>
#include "Board_properties.hpp"
#include "tank_drawable.hpp"
#include "button.hpp"

const size_t COUNT_DOWN = 5;

class game final {
    TFT_eSPI& tft_;

    std::vector<std::unique_ptr<Tank_d>> tanks_;
    Button buttons_[BTN_COUNT];

    public:
        game(TFT_eSPI& tft) : tft_(tft),
        buttons_{
            Button(BTN_UP_PIN),     // BTN_UP
            Button(BTN_DOWN_PIN),   // BTN_DOWN
            Button(BTN_LEFT_PIN),   // BTN_LEFT
            Button(BTN_RIGHT_PIN),  // BTN_RIGHT
            Button(BTN_X_PIN),      // BTN_X
            Button(BTN_Y_PIN),      // BTN_Y
            Button(BTN_A_PIN),      // BTN_A
            Button(BTN_B_PIN),      // BTN_B
            Button(BTN_PAUSA_PIN)   // BTN_PAUSA
        } {};

        void start(void);

        void check_updates_buttons(void); // function to update the status of buttons, can be used in the main loop to check for button presses
        void execute_updates();

        void create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed); 
        void delete_tank(size_t index);
        void delete_enemy_tanks(void); 

        game& operator=(game& other) = delete; // delete copy assignment operator
        game(game& other)  = delete; // delete copy constructor
        game(game&& other) = delete; // delete move constructor
};

void CountDown(TFT_eSPI& tft);

#endif