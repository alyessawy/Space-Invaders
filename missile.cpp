#include "missile.h"

const uint32_t Missile::sprite[] = {
 0x00000000, 0xff000000, 0x00000000, 
0xff000000, 0xff000000, 0xff000000, 
0x00000000, 0xff000000, 0x00000000, 
0x00000000, 0xff000000, 0x00000000, 
0xff000000, 0xff000000, 0xff000000, 
0x00000000, 0xff000000, 0x00000000, 
0xff000000, 0xff000000, 0xff000000
};

Missile::Missile() : x(0), y(0), active(false) {}

void Missile::init(int start_x, int start_y) {
    x = start_x;
    y = start_y;
    active = false;
}

void Missile::update() {
    if (active) {
        y -= 3; 
        if (y < 0) {
            active = false; 
        }
    }
}

void Missile::draw(N5110 &lcd) {
    if (active) {
        lcd.drawSprite(x, y, height, width, (uint32_t *)sprite);
    }
}

void Missile::fire(int x_pos, int y_pos) {
    x = x_pos;
    y = y_pos;
    active = true;
}

bool Missile::is_active() const { return active; }
int Missile::get_x() const { return x; }
int Missile::get_y() const { return y; }
int Missile::get_width() const { return width; }
int Missile::get_height() const { return height; }



