#ifndef CLASS_TANK_DRAW
#define CLASS_TANK_DRAW

#include <iostream>
#include <TFT_eSPI.h> 
#include "./Board_properties.hpp"
#include "./Textures/default_tank.h"
#include "Drawable.hpp"



class Tank_d : public Drawable {
  const size_t max_health_, max_ammunition_; 
  size_t health_, ammunition_, speed_; 

  bool is_valid_;

  inline static const uint16_t* skins[4] = {
    default_tank_up, 
    default_tank_down, 
    default_tank_left, 
    default_tank_right 
  };

  int tank_type; // Индекс скина для конкретного экземпляра

  TFT_eSPI& tft_;
  //std::unique_ptr<TFT_eSprite> tank_sprite_;

  public:
    Tank_d(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed, TFT_eSPI& tft) : 
    Drawable(x_pos, y_pos, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT),
    tft_(tft), speed_(speed), max_health_(health), health_(health),
    max_ammunition_(ammunition), ammunition_(ammunition), is_valid_(false) { 
    //   if (ESP.getFreeHeap() < 5000) {
    //     Serial.println("Недостаточно памяти для танка");
    //     return;
    //   }
  
    //   tank_sprite_ = std::make_unique<TFT_eSprite>(&tft);
    //   if (!tank_sprite_) {
    //     return;
    //   }  

      background_buffer = std::make_unique<uint16_t[]>(width*height);
      if (!background_buffer) {
        return;
      }  

    //   tank_sprite_->createSprite(width, height);
    //   tank_sprite_->setSwapBytes(true);
    //   tank_sprite_->pushImage(0, 0, width, height, default_tank_up);
    
      // storing buffer of the background pixels before drawing the tank  
      tft.readRect(x_pos, y_pos, width, height, background_buffer.get()); 

      is_valid_ = true;
    };  

    void draw() override;
    bool is_valid() {return is_valid_;}  

    void update_orientation(int dx, int dy);    

    size_t get_speed()          const {return speed_;}
    size_t get_health()         const {return health_;}
    size_t get_max_health()     const {return max_health_;}
    size_t get_ammunition()     const {return ammunition_;}
    size_t get_max_ammunition() const {return max_ammunition_;}
};
#endif