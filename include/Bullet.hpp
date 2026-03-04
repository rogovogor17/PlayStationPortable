#include "./Textures/bullet/bullet.hpp"
#include "Drawable.hpp"

constexpr size_t default_bullet_speed = 15;

class Bullet: public Drawable {
  size_t speed_;
  int dx, dy;
  TFT_eSPI& tft_;

  bool is_valid_;

  inline static const uint16_t* skins[4] = {
    bullet_up, 
    bullet_down, 
    bullet_left, 
    bullet_right, 
  };

  public:
    Bullet(int x, int y, int orient, size_t speed, TFT_eSPI& tft): Drawable(x, y, default_bullet_length, default_bullet_width), 
    speed_(speed), is_valid_(false), tft_(tft) {
        setOrientation(orient);
        switch(orientation) {
            case DIR_UP:    dx =      0, dy = -speed; break;
            case DIR_DOWN:  dx =      0, dy =  speed; break;
            case DIR_RIGHT: dx =  speed, dy =      0; break;
            case DIR_LEFT:  dx = -speed, dy =      0; break;
            default: break;
        }

        background_buffer = std::make_unique<uint16_t[]>(width*height);  

        if (!background_buffer) {
            return;
        }    
        // storing buffer of the background pixels before drawing the tank  
        tft.readRect(pos_x, pos_y, width, height, background_buffer.get());   
        is_valid_ = true;
    }

    void draw() override {
      restore_background(tft_); // Using class drawable
      save_background(tft_);    // Using class drawable  

      old_x = pos_x;
      old_y = pos_y;
      
      const uint16_t* current_sprite = skins[orientation];
      tft_.setSwapBytes(true); 
      tft_.pushImage(pos_x, pos_y, width, height, current_sprite, TRANSPARENT_COLOR);
    }

    int get_dx() {return dx;}
    int get_dy() {return dy;}
};