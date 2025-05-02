#ifndef ALIEN_H
#define ALIEN_H

#include "mbed.h"
#include "N5110.h"
#include "alien_sprites.h"
#include "laser.h"

using namespace std::chrono;

class Alien {
public:
    Alien();
    void init(int rows, int cols);
    void set_difficulty(int diff); 
    void update();
    void draw(N5110 &lcd);
    void reset();
    int check_collision(int x, int y, int width, int height);
    int check_missile_collision(int x, int y, int width, int height);
    void remove_alien(int index);
    int get_alien_count() const;
    int get_points_for_alien(int index) const;
    int get_alien_height() const { return get_alien_height_for_type(0); } 
    bool alien_at_bottom(int screen_bottom) const;
    void fire_laser(Laser* lasers, int max_lasers);
    void update_lasers(Laser* lasers, int max_lasers);
    bool check_spaceship_collision(int ship_x, int ship_y, int ship_width, int ship_height);
    
private:
    struct AlienData {
        int x;
        int y;
        int type; 
        bool active;
    };
    
    static const int MAX_ALIENS = 12; 
    AlienData aliens[MAX_ALIENS];
    int active_aliens = 0;
    int difficulty = 1; 
    
    Timer animation_timer;
    Timer move_timer;
    Timer laser_timer;
    bool current_frame;
    int direction;
    int move_down_counter;
    
    static const int ALIEN_SPACING = 12;
    static const int ALIEN_START_X = 10;
    static const int ALIEN_START_Y = 0;
    static const int ALIEN_MOVE_DELAY = 500;
    static const int LASER_FIRE_DELAY = 2000;
    
    int get_alien_width_for_type(int type) const;
    int get_alien_height_for_type(int type) const;
    const uint32_t* get_alien_sprite(int type) const;
    void move_aliens();
};

#endif