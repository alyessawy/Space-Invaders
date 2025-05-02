#include "laser.h"

const uint32_t Laser::sprite[] = {
    0xff000000,
    0xff000000,
    0xff000000,
    0xff000000,
    0xff000000,
    0xff000000,
    0xff000000,
    0xff000000
};


Laser::Laser() : x(0), y(0), active(false) {}

void Laser::init(int start_x, int start_y) {
    x = start_x;
    y = start_y;
    active = false;
}

void Laser::update(bool move_up) {
    if (active) {
        if (move_up) {
            y -= 3; 
            if (y < 0) {
                active = false; 
            }
        } else {
            y += 3; 
            if (y > 48) { 
                active = false; 
            }
        }
    }
}

void Laser::draw(N5110 &lcd) {
    if (active) {
        lcd.drawSprite(x, y, height, width, (uint32_t *)sprite);
    }
}

void Laser::fire(int x_pos, int y_pos) {
    x = x_pos;
    y = y_pos;
    active = true;
}

bool Laser::is_active() const { return active; }
int Laser::get_x() const { return x; }
int Laser::get_y() const { return y; }
int Laser::get_width() const { return width; }
int Laser::get_height() const { return height; }