#include "./Textures/bullet/bullet.hpp"
#include "Drawable.hpp"
#include "Collidable.hpp"

constexpr size_t default_bullet_speed = 15;

enum class BulletState {
  Active,   
  Exploding, 
  Dead  
};

class Bullet: public Drawable, public Collidable {
  size_t speed_;
  int dx, dy;
  TFT_eSPI& tft_;

  bool is_valid_;
  int skintype_ = 0;

  //explosion
  int explosion_timer_ = 0;      
  const int EXPLOSION_DURATION = 10; //num of ticks
  int num_of_frame_explosure = 0;
  BulletState state_;
  

  inline static const uint16_t* skins[2][4] = {
    {bullet_up, 
    bullet_down, 
    bullet_left, 
    bullet_right}, 
    {bullet_bomb,  bullet_bomb,  bullet_bomb,  bullet_bomb} //respond for explosion
  };

  public:
    Bullet(int x, int y, int orient, size_t speed, TFT_eSPI& tft): Drawable(x, y, default_bullet_length, default_bullet_width), 
    speed_(speed), is_valid_(false), tft_(tft), state_(BulletState::Active) {
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
      
      const uint16_t* current_sprite = skins[skintype_][orientation];
      tft_.setSwapBytes(true); 
      tft_.pushImage(pos_x, pos_y, width, height, current_sprite, TRANSPARENT_COLOR);
    }

    bool animation_finished() const {return num_of_frame_explosure == EXPLOSION_DURATION;}
    void mark_dead() {state_ = BulletState::Dead;}
    void mark_exploding() {skintype_ = 1; state_ = BulletState::Exploding;}

    int get_dx() {return dx;}
    int get_dy() {return dy;}

    Rect get_collision_rect() const override {
      return {pos_x, pos_y, width, height};
    }

    void on_collision(Collidable* other) override {
      auto type = other->get_type();
      
      switch (type) {
        case CollidableType::BULLET:          
        case CollidableType::WALL:
        case CollidableType::TANK:
          mark_exploding();
            
        default:  
          break;
      }
    }

    void update() override {
      if (state_ == BulletState::Exploding) {
        explosion_timer_++;
        
        if (explosion_timer_ >= EXPLOSION_DURATION){ 
          state_ = BulletState::Dead;
        }
      }
      
      if (state_ ==  BulletState::Active) {}
    }

    CollidableType get_type() const override {
      return CollidableType::BULLET;
    }

    bool is_active() const override {
      return state_ == BulletState::Active;
    }

    bool is_exploding() {
      return state_ == BulletState::Exploding;
    }

    bool is_dead() const {
      return state_ == BulletState::Dead;
    }
};