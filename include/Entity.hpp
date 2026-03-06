#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <TFT_eSPI.h>
#include "Map.hpp"

struct Rect { 
  int x, y, w, h; 
};

enum Direction {
  DIR_UP   ,
  DIR_DOWN ,
  DIR_LEFT , 
  DIR_RIGHT,
};

enum class CollidableType {
  TANK,
  BULLET,
  WALL, 
  NONE
};

constexpr uint16_t TRANSPARENT_COLOR = 0x0000; // white light considers transparent

class Entity {
  protected:
    int pos_x, pos_y, old_x, old_y;
    std::unique_ptr<uint16_t[]> background_buffer = nullptr;  // buffer to store the background pixels before drawing the tank
    int width, height;
    
    bool active;        // активен ли объект (true - рисовать/обновлять)
    bool visible;       // видим ли объект

    int orientation; // start from up and increases by turning to the left in degrees
    
  public:
     Entity(int startX = 0, int startY = 0, int w = 10, int h = 10) : 
      pos_x(startX), 
      pos_y(startY), 
      old_x(startX), 
      old_y(startY),
      width(w), 
      height(h), 
      active(true), 
      visible(true),
      orientation(0) {}

    virtual ~Entity() {}
    
    //inheritor have to realize this functions 
    virtual void draw()   = 0;

    void restore_background(TFT_eSPI& tft) {
      tft.setSwapBytes(false); 
      tft.pushImage(old_x, old_y, width, height, background_buffer.get());
    }

    void save_background(TFT_eSPI& tft) {
      tft.setSwapBytes(false); 
      tft.readRect(pos_x, pos_y, width, height, background_buffer.get());
    }
    
    int getX()      { return pos_x; }
    int getY()      { return pos_y; }
    int getWidth()  { return width; }
    int getHeight() { return height; }
    int getOrientation() { return orientation; }
    
    void setPosition(int newX, int newY) {
      pos_x = newX;
      pos_y = newY;
    }

    void setOrientation(int newOrientation) {
      orientation = newOrientation;
    }
    
    void move(int dx, int dy) {
      //onBeforeMove();
      pos_x += dx;
      pos_y += dy;

      //onAfterMove();
    }
    
    bool isActive() { return active; }
    void setActive(bool state) { active = state; }
    
    bool isVisible() { return visible; }
    void setVisible(bool state) { visible = state; }
    
    // Проверка столкновения с другим объектом
    bool collidesWith(int dx, int dy,  Entity* other) {
      return ((pos_x + dx + width  > other->pos_x  &&
               pos_y + dy + height > other->pos_y) &&
              (pos_x + dx < other->pos_x + other->width &&
               pos_y + dy < other->pos_y + other->height)
            );
    }
    
    // Проверка, находится ли точка внутри объекта
    bool containsPoint(int px, int py) {
      return (px >= pos_x && px <= pos_x + width &&
        py >= pos_y && py <= pos_y + height);
    }

  
  virtual Rect get_collision_rect() const = 0;
  //describes what object will do if collids with anothe object
  virtual void on_collision(Entity* other) = 0; 
  //responsible for being accounted as a collidable object
  virtual bool is_active() const = 0; 
  virtual Entity* get_owner() const { return nullptr; }
  virtual void update() = 0;
  virtual CollidableType get_type() const = 0;
};

class MapWallEntity : public Entity {
public:
    MapWallEntity() : Entity(0, 0, TILE_SIZE, TILE_SIZE) {}
    void draw() override {} // Стены рисует Game::draw_map_part
    void update() override {}
    bool is_active() const override { return true; }
    CollidableType get_type() const override { return CollidableType::WALL; }
    Rect get_collision_rect() const override { return {0,0,0,0}; } // Не используется
    void on_collision(Entity* other) override {} 
};

#endif