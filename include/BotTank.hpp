#pragma once
#include "Tank.hpp"

enum class BotType {
    easy,
    normal, 
    hard,
};

class BotTank : public Tank {
private:
    BotType type_ = BotType::easy;
    
    unsigned long last_shot_time_     = 0;
    unsigned long last_decision_time_ = 0;
    unsigned long last_move_time_     = 0;
    
    const int DECISION_INTERVAL = 500;
    
    void make_decision();
    void handle_movement();
    
public:

    bool fired_ = false;

    void update() override { 
        Tank::update();
        handle_movement();
    }

    BotTank(int x, int y, int health, int ammo, int speed, TFT_eSPI& tft)
        : Tank(x, y, health, ammo, speed, tft) {}
    
    void set_type(const BotType& type);

    void shoot() {fired_ = true;}
    bool wants_to_shoot();
    void on_shot_fired() { last_shot_time_ = millis(); }
};