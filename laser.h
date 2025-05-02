#ifndef LASER_H
#define LASER_H

#include "mbed.h"
#include "N5110.h"

class Laser {
public:
    Laser();
    void init(int start_x, int start_y);
    void update(bool move_up = true); 
    void draw(N5110 &lcd);
    void fire(int x, int y);
    bool is_active() const;
    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;
    
private:
    int x;
    int y;
    bool active;
    static const int width = 1;
    static const int height = 8;
    static const uint32_t sprite[];
};

#endif