#include "./Textures/bullet/bullet.hpp"
#include "Entity.hpp"

constexpr size_t default_bullet_speed = 15;

enum class BulletState {
  Active,   
  Exploding, 
  Dead  
};

class Bullet: public Entity {
  TFT_eSPI& tft_;

  size_t speed_;
  int dx, dy;
  bool is_valid_;
  int skintype_ = 0;

  Entity* owner_;
  //explosion
  int explosion_timer_ = 0;      
  const int EXPLOSION_DURATION = 10; //num of ticks
  BulletState state_;
  

  inline static const uint16_t* skins[2][4] = {
    {bullet_up, 
    bullet_down, 
    bullet_left, 
    bullet_right}, 
    {bullet_bomb,  bullet_bomb,  bullet_bomb,  bullet_bomb} //respond for explosion
  };

  public:
    Bullet(int x, int y, Entity* entity, size_t speed, TFT_eSPI& tft): 
    Entity(x, y, default_bullet_length, default_bullet_width), 
    speed_(speed), is_valid_(false), tft_(tft), state_(BulletState::Active) {
        owner_ = entity;
        setOrientation(entity->getOrientation());
        switch(orientation) {
            case DIR_UP:    dx =      0, dy = -speed; break;
            case DIR_DOWN:  dx =      0, dy =  speed; break;
            case DIR_RIGHT: dx =  speed, dy =      0; break;
            case DIR_LEFT:  dx = -speed, dy =      0; break;
            default: break;
        }

        // background_buffer = std::make_unique<uint16_t[]>(width*height);  

        // if (!background_buffer) {
        //     return;
        // }    
        // storing buffer of the background pixels before drawing the tank  
        //tft.readRect(pos_x, pos_y, width, height, background_buffer.get());   
        is_valid_ = true;
    }

    void draw() override {
      //restore_background(tft_); // Using class drawable
      //save_background(tft_);    // Using class drawable  

      old_x = pos_x;
      old_y = pos_y;
      
      const uint16_t* current_sprite = skins[skintype_][orientation];
      tft_.setSwapBytes(true); 
      tft_.pushImage(pos_x, pos_y, width, height, current_sprite, TRANSPARENT_COLOR);
    }

    bool animation_finished() const {return explosion_timer_ >= EXPLOSION_DURATION;}
    void mark_dead() {state_ = BulletState::Dead;}
    void mark_exploding() {skintype_ = 1; explosion_timer_ = 0; state_ = BulletState::Exploding;}

    Entity* get_owner() const override { return owner_;}
    int get_dx() {return dx;}
    int get_dy() {return dy;}

    Rect get_collision_rect() const override {
      return {pos_x, pos_y, width, height};
    }

    void on_collision(Entity* other) override {
      //if (other == owner) return;
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
      }
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