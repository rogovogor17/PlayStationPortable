#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <TFT_eSPI.h>

class Drawable {
  protected:

    int x, y;
    int width, height;
    
    bool active;        // активен ли объект (true - рисовать/обновлять)
    bool visible;       // видим ли объект
    
  public:
    Drawable(int startX = 0, int startY = 0, int w = 10, int h = 10) : 
        x(startX), 
        y(startY), 
        width(w), 
        height(h), 
        active(true), 
        visible(true) {}

    virtual ~Drawable() {}
    
    //inheritor have to realize this functions 
    virtual void draw(TFT_eSPI &tft) = 0;
    virtual void update() = 0;
    
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    
    void setPosition(int newX, int newY) {
      x = newX;
      y = newY;
    }
    
    void move(int dx, int dy) {
      x += dx;
      y += dy;
    }
    
    bool isActive() { return active; }
    void setActive(bool state) { active = state; }
    
    bool isVisible() { return visible; }
    void setVisible(bool state) { visible = state; }
    
    // Проверка столкновения с другим объектом
    bool collidesWith(Drawable* other) {
      return (x < other->x + other->width &&
        x + width > other->x &&
        y < other->y + other->height &&
        y + height > other->y);
    }
    
    // Проверка, находится ли точка внутри объекта
    bool containsPoint(int px, int py) {
      return (px >= x && px <= x + width &&
        py >= y && py <= y + height);
    }
};

#endif