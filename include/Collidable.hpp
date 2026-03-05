#pragma once

struct Rect { 
    int x, y, w, h; 
};

enum class CollidableType {
    TANK,
    BULLET,
    WALL, 
    NONE
};

class Collidable {
public:
    virtual Rect get_collision_rect() const = 0;
    //describes what object will do if collids with anothe object
    virtual void on_collision(Collidable* other) = 0; 
    //responsible for being accounted as a collidable object
    virtual bool is_active() const = 0; 
    virtual int get_owner_id() const { return -1; }
    virtual void update() = 0;
    virtual CollidableType get_type() const = 0;
};