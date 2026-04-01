#ifndef GAME_CLASS
#define GAME_CLASS

#include <SPI.h>
#include <TFT_eSPI.h> 
#include <vector>
#include <memory>
#include "CollisionManager.hpp"
#include "Board_properties.hpp"
#include "Tank.hpp"
#include "Button.hpp"
#include "Map.hpp"

const size_t COUNT_DOWN = 5;

class Game final {
    TFT_eSPI& tft_;

    CollisionManager collision_mgr_;

    std::vector<std::shared_ptr<Tank>> tanks_;
    std::vector<std::shared_ptr<Bullet>> bullets_;

    std::unique_ptr<uint16_t[]> full_screen_buffer_;

    Button buttons_[BTN_COUNT];

    bool is_running_ = false;

    public:
        Game(TFT_eSPI& tft) : tft_(tft),
        buttons_{
            Button(BTN_UP_PIN),     // BTN_UP
            Button(BTN_DOWN_PIN),   // BTN_DOWN
            Button(BTN_LEFT_PIN),   // BTN_LEFT
            Button(BTN_RIGHT_PIN),  // BTN_RIGHT
            Button(BTN_X_PIN),      // BTN_X
            Button(BTN_Y_PIN),      // BTN_Y
            Button(BTN_A_PIN),      // BTN_A
            Button(BTN_B_PIN),      // BTN_B
            Button(BTN_PAUSA_PIN)}   // BTN_PAUSA 
        {
            collision_mgr_.set_wall_destroyed_callback(
                [&tft](int x, int y) {
                    tft.fillRect(x, y, TILE_SIZE, TILE_SIZE, BLACK);
                }
            );
        };

        void start(void);

        // function to update the status of buttons, can be used in the main loop to check for button presses
        void draw_map();
        void draw_map_part(Rect r);

        void check_updates_buttons(void); //input_manager
        void execute_updates();

        void register_collidables();

        void create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed); 
        void delete_tank(size_t index);
        void delete_enemy_tanks(void); 
        void create_flying_bullet();
        bool is_out_of_bounds (Rect next);
        void cleanup_dead_objects();

        // void render() {
        //     renderer_.clear();
        //     renderer_.draw_map(walls_);
        //     renderer_.draw_tanks(tanks_);
        //     renderer_.draw_bullets(bullets_);
        //     renderer_.display();
        // }

        Game& operator=(Game& other) = delete; // delete copy assignment operator
        Game(Game& other)  = delete; // delete copy constructor
        Game(Game&& other) = delete; // delete move constructor

        void save_full_background() {
        tft_.readRect(0, 0, X_MAX, Y_MAX, full_screen_buffer_.get());
        }
        
        void restore_full_background() {
            tft_.pushImage(0, 0, X_MAX, Y_MAX, full_screen_buffer_.get());
        }
};

void CountDown(TFT_eSPI& tft);

#endif