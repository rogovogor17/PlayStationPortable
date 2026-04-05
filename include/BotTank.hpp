#pragma once
#include "Tank.hpp"

enum class BotType {
    easy,
    normal, 
    hard,
};

enum class AIState {
    CHASE, ATTACK 
};

class BotTank : public Tank {
private:
    std::shared_ptr<Tank> target_;
    BotType type_ = BotType::easy;
    
    unsigned long last_shot_time_ = 0;
    unsigned long last_decision_time_ = 0;
    unsigned long last_move_time_ = 0;
    
    const int DECISION_INTERVAL = 500;
    const int MOVE_INTERVAL = 50;
    
    AIState state_ = AIState::CHASE;
    
    void make_decision();
    void chase_target();
    
    void handle_movement();
    
    
public:

    void update() override { 
        Tank::update();
        handle_movement();
    }

    BotTank(int x, int y, int health, int ammo, int speed, TFT_eSPI& tft)
        : Tank(x, y, health, ammo, speed, tft) {}
    
    void set_target(std::shared_ptr<Tank> target) { target_ = target; }
    void set_type(const BotType& type);
    
    bool wants_to_shoot();
    void on_shot_fired() { last_shot_time_ = millis(); }
};