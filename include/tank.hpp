#ifndef CLASS_TANK_DRAW
#define CLASS_TANK_DRAW

#include <iostream>
#include <TFT_eSPI.h> 
#include "./Board_properties.hpp"
#include "./Textures/default_tank/default_tank.h"
#include "Entity.hpp"
#include "Bullet.hpp"

enum class TankState {
  Active,   
  Exploding, 
  Dead  
};

class Tank : public Entity {
  const size_t max_health_, max_ammunition_; 
  size_t health_, ammunition_, speed_; 

  bool is_valid_;
  bool active_ = true;
  TankState state_ = TankState::Active;

  inline static const uint16_t* skins[4] = {
    default_tank_up, 
    default_tank_down, 
    default_tank_left, 
    default_tank_right 
  };

  int tank_type; // Индекс скина для конкретного экземпляра

  TFT_eSPI& tft_;

  public:
    Tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed, TFT_eSPI& tft) : 
    Entity(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT),
    tft_(tft), speed_(speed), max_health_(health), health_(health),

    max_ammunition_(ammunition), ammunition_(ammunition), is_valid_(false) { 

      // background_buffer = std::make_unique<uint16_t[]>(width*height);
      // if (!background_buffer) {
      //   return;
      // }  
      // // storing buffer of the background pixels before drawing the tank  
      // tft.readRect(x_pos, y_pos, width, height, background_buffer.get()); 

      is_valid_ = true;
    };  

    void draw() override;
    bool is_valid() {return is_valid_;}  
    void update() override {} 

    Entity* get_owner() const override { return nullptr; }

    Rect get_collision_rect() const override {
      return {pos_x, pos_y, width, height};
    }

    void on_collision(Entity* other) override {
      auto type = other->get_type();
      
      switch (type) {
        case CollidableType::BULLET:
          health_ -= 10;
          if (health_ <= 0) {
            active_ = false;
          }
          break;
            
        case CollidableType::WALL:
          break;

        case CollidableType::TANK:
          break;
            
        default:  
          break;
      }
    }

    CollidableType get_type() const override {
      return CollidableType::TANK;
    }

    bool is_active()    const override {return state_ == TankState::Active;}
    bool is_exploding() const {return state_ == TankState::Exploding;}
    bool is_dead()      const {return state_ == TankState::Dead;}

    void kill() {active_ = false;} 

    void update_orientation(int dx, int dy);

    std::pair<int, int> count_nose_of_the_tank(int bullet_width, int bullet_length) {
      switch(orientation) {
        case DIR_UP:    return std::pair<int, int>(pos_x + width/2 - bullet_width/2, pos_y - bullet_length);  break;
        case DIR_DOWN:  return std::pair<int, int>(pos_x + width/2 - bullet_width/2, pos_y + height);         break;
        case DIR_LEFT:  return std::pair<int, int>(pos_x - bullet_length, pos_y + height/2 - bullet_width/2); break;
        case DIR_RIGHT: return std::pair<int, int>(pos_x + width, pos_y + height/2 - bullet_width/2);         break;
      }

      return std::pair{0, 0};
    }    

    size_t get_speed()          const {return speed_;}
    size_t get_health()         const {return health_;}
    size_t get_max_health()     const {return max_health_;}
    size_t get_ammunition()     const {return ammunition_;}
    size_t get_max_ammunition() const {return max_ammunition_;}
};

#endif