#ifndef MISSILE_H
#define MISSILE_H

#include "mbed.h"
#include "N5110.h"

class Missile {
public:
    Missile();
    void init(int start_x, int start_y);
    void update();
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
    static const int width = 3;
    static const int height = 7;
    static const uint32_t sprite[];
};

#endif


