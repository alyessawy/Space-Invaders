#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "mbed.h"
#include "N5110.h"
#include "utils.h"  

class Spaceship {
public:
    Spaceship();
    void init(int start_x, int start_y);
    void draw(N5110 &lcd);
    void update(UserInput input);
    Position2D get_pos();
    const uint32_t* get_sprite() const;
    int get_width() const;
    int get_height() const;

private:
    int _x ;
    int _y ;
    int _speed;
    static const int _width = 11;
    static const int _height = 11;
    static const uint32_t _sprite[];
};

#endif

