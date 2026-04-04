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

constexpr size_t COUNT_DOWN = 5;

enum class GameStatus {
    IN_PROGRESS,
    ON_HOLD,
    OVER,
};

class Game final {
    TFT_eSPI& tft_;
    GameStatus status_ = GameStatus::IN_PROGRESS;
    CollisionManager collision_mgr_;
    uint8_t game_map_[MAP_HEIGHT][MAP_WIDTH];

    std::vector<std::shared_ptr<Tank>> tanks_;
    std::vector<std::shared_ptr<Bullet>> bullets_;

    std::unique_ptr<uint16_t[]> full_screen_buffer_;

    Button buttons_[BTN_COUNT];

    bool is_running_ = true;

    public:
        Game(TFT_eSPI& tft) : tft_(tft), collision_mgr_(game_map_),
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
            memcpy(game_map_, map_level_1, sizeof(game_map_));   

            collision_mgr_.set_wall_destroyed_callback(
                [&tft](int x, int y) {
                    tft.fillRect(x, y, TILE_SIZE, TILE_SIZE, TFT_BLACK);
                }
            );

            collision_mgr_.set_base_destroyed_callback(
                [this]() { 
                    status_ = GameStatus::OVER;  
                }
            );
        };

        void start(void);

        // function to update the status of buttons, can be used in the main loop to check for button presses
        void draw_map();
        void draw_map_part(Rect r);
        void draw_pause_screen();
        void print_tank_data_to_info_table(const Tank& tank, bool force_update = false);

        void check_updates_buttons(void); //input_manager
        void execute_updates();

        void register_collidables();

        void create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed); 
        void delete_tank(size_t index);
        void delete_enemy_tanks(void); 
        void create_flying_bullet();
        bool is_out_of_bounds (Rect next);
        void cleanup_dead_objects();

        GameStatus get_status() const noexcept {return status_;}
        bool   is_in_progress() const noexcept {return status_ == GameStatus::IN_PROGRESS;}

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