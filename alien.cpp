#include "alien.h"


Alien::Alien() : current_frame(false), direction(1), move_down_counter(0), active_aliens(0) {
    animation_timer.start();
    move_timer.start();
    laser_timer.start();
}

void Alien::set_difficulty(int diff) {
    difficulty = diff;
}

int Alien::get_alien_width_for_type(int type) const {
    switch(type) {
        case 0: return 10;  
        case 1: return 10;  
        case 2: return 8;   
        default: return 10;
    }
}

int Alien::get_alien_height_for_type(int type) const {
    switch(type) {
        case 0: return 8;  
        case 1: return 8;  
        case 2: return 8;  
        default: return 8;
    }
}

void Alien::init(int rows, int cols) {
    active_aliens = 12; 
    if (active_aliens > MAX_ALIENS) active_aliens = MAX_ALIENS;
    
    int index = 0;
    
    for (int col = 0; col < 5 && index < MAX_ALIENS; col++) {
        aliens[index].x = ALIEN_START_X + col * ALIEN_SPACING;
        aliens[index].y = ALIEN_START_Y;
        aliens[index].type = col % 3;
        aliens[index].active = true;
        index++;
    }
    
    for (int col = 0; col < 4 && index < MAX_ALIENS; col++) {
        aliens[index].x = ALIEN_START_X + col * ALIEN_SPACING + ALIEN_SPACING / 2; 
        aliens[index].y = ALIEN_START_Y + (ALIEN_SPACING - 2);
        aliens[index].type = col % 3;
        aliens[index].active = true;
        index++;
    }
    
    for (int col = 0; col < 3 && index < MAX_ALIENS; col++) {
        aliens[index].x = ALIEN_START_X + col * ALIEN_SPACING + ALIEN_SPACING; 
        aliens[index].y = ALIEN_START_Y + 2 * (ALIEN_SPACING - 2);
        aliens[index].type = col % 3;
        aliens[index].active = true;
        index++;
    }
    move_down_counter = 0;
    direction = 1;
    laser_timer.reset();
    move_timer.reset();
}

void Alien::update() {
    if (duration_cast<milliseconds>(animation_timer.elapsed_time()).count() > 1000) {
        current_frame = !current_frame;
        animation_timer.reset();
    }
    
    if (duration_cast<milliseconds>(move_timer.elapsed_time()).count() > ALIEN_MOVE_DELAY) {
        move_aliens();
        move_timer.reset();
    }
}

void Alien::draw(N5110 &lcd) {
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            const uint32_t* sprite = get_alien_sprite(aliens[i].type);
            int width = get_alien_width_for_type(aliens[i].type);
            int height = get_alien_height_for_type(aliens[i].type);
            lcd.drawSprite(aliens[i].x, aliens[i].y, height, width, (uint32_t*)sprite);
        }
    }
}

void Alien::reset() {
    animation_timer.stop();
    animation_timer.reset();
    move_timer.stop();
    move_timer.reset();
    laser_timer.stop();
    laser_timer.reset();
    current_frame = false;
    direction = 1;
    move_down_counter = 0;
}

int Alien::check_collision(int x, int y, int width, int height) {
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            int alien_width = get_alien_width_for_type(aliens[i].type);
            int alien_height = get_alien_height_for_type(aliens[i].type);
            
            if (x < aliens[i].x + alien_width &&
                x + width > aliens[i].x &&
                y < aliens[i].y + alien_height &&
                y + height > aliens[i].y) {
                return i;
            }
        }
    }
    return -1;
}

int Alien::check_missile_collision(int x, int y, int width, int height) {
    int aliens_hit = 0;
    int hit_index = -1;
    
    
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            int alien_width = get_alien_width_for_type(aliens[i].type);
            int alien_height = get_alien_height_for_type(aliens[i].type);
            
            if (x < aliens[i].x + alien_width &&
                x + width > aliens[i].x &&
                y < aliens[i].y + alien_height &&
                y + height > aliens[i].y) {
                hit_index = i;
                break;
            }
        }
    }
    
    if (hit_index != -1) {
        
        aliens[hit_index].active = false;
        aliens_hit++;
        
        
        if (hit_index < 5) {
            
            if (hit_index > 0 && aliens_hit < 4) {
                
                if (aliens[hit_index - 1].active) {
                    aliens[hit_index - 1].active = false;
                    aliens_hit++;
                }
            }
            if (hit_index < 4 && aliens_hit < 4) {
                
                if (aliens[hit_index + 1].active) {
                    aliens[hit_index + 1].active = false;
                    aliens_hit++;
                }
            }
        } else if (hit_index < 9) {
            
            if (hit_index > 5 && aliens_hit < 4) {
                
                if (aliens[hit_index - 1].active) {
                    aliens[hit_index - 1].active = false;
                    aliens_hit++;
                }
            }
            if (hit_index < 8 && aliens_hit < 4) {
                
                if (aliens[hit_index + 1].active) {
                    aliens[hit_index + 1].active = false;
                    aliens_hit++;
                }
            }
            if (aliens_hit < 4) {
                
                int up_index = hit_index - 5;
                if (up_index >= 0 && aliens[up_index].active) {
                    aliens[up_index].active = false;
                    aliens_hit++;
                }
            }
        } else {
            
            if (hit_index > 9 && aliens_hit < 4) {
                
                if (aliens[hit_index - 1].active) {
                    aliens[hit_index - 1].active = false;
                    aliens_hit++;
                }
            }
            if (hit_index < 11 && aliens_hit < 4) {
                
                if (aliens[hit_index + 1].active) {
                    aliens[hit_index + 1].active = false;
                    aliens_hit++;
                }
            }
            if (aliens_hit < 4) {
                
                int up_index = hit_index - 4;
                if (up_index >= 5 && aliens[up_index].active) {
                    aliens[up_index].active = false;
                    aliens_hit++;
                }
            }
        }
    }
    
    return aliens_hit;
}

bool Alien::check_spaceship_collision(int ship_x, int ship_y, int ship_width, int ship_height) {
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            int alien_width = get_alien_width_for_type(aliens[i].type);
            int alien_height = get_alien_height_for_type(aliens[i].type);
            
            if (ship_x < aliens[i].x + alien_width &&
                ship_x + ship_width > aliens[i].x &&
                ship_y < aliens[i].y + alien_height &&
                ship_y + ship_height > aliens[i].y) {
                return true;
            }
        }
    }
    return false;
}

void Alien::remove_alien(int index) {
    if (index >= 0 && index < active_aliens) {
        aliens[index].active = false;
    }
}

int Alien::get_alien_count() const {
    int count = 0;
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) count++;
    }
    return count;
}

int Alien::get_points_for_alien(int index) const {
    if (index >= 0 && index < active_aliens) {
        switch(aliens[index].type) {
            case 0: return 10;  
            case 1: return 20;  
            case 2: return 30;  
            default: return 0;
        }
    }
    return 0;
}

bool Alien::alien_at_bottom(int screen_bottom) const {
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            int alien_height = get_alien_height_for_type(aliens[i].type);
            if (aliens[i].y + alien_height >= screen_bottom) {
                return true;
            }
        }
    }
    return false;
}

const uint32_t* Alien::get_alien_sprite(int type) const {
    switch(type) {
        case 0: return current_frame ? alien1_frame_first : alien1_frame_second;
        case 1: return alien2;
        case 2: return alien3;
        default: return alien1_frame_first;
    }
}

void Alien::move_aliens() {
    int left = 84, right = 0;
    for (int i = 0; i < active_aliens; i++) {
        if (aliens[i].active) {
            int alien_width = get_alien_width_for_type(aliens[i].type);
            left = std::min(left, aliens[i].x);
            right = std::max(right, aliens[i].x + alien_width);
        }
    }
    
    if ((direction == 1 && right >= 84) || (direction == -1 && left <= 0)) {
        direction *= -1;
        move_down_counter++;
        
        for (int i = 0; i < active_aliens; i++) {
            if (aliens[i].active) {
                aliens[i].y += 5;
            }
        }
    } else {
        for (int i = 0; i < active_aliens; i++) {
            if (aliens[i].active) {
                aliens[i].x += direction * 2;
            }
        }
    }
}

void Alien::fire_laser(Laser* lasers, int max_lasers) {
    if (difficulty == 0) return; 
    
    int fire_delay = LASER_FIRE_DELAY;
    if (difficulty == 1) fire_delay = LASER_FIRE_DELAY * 2; 
    else if (difficulty == 2) fire_delay = LASER_FIRE_DELAY / 2; 
    
    if (duration_cast<milliseconds>(laser_timer.elapsed_time()).count() < fire_delay) {
        return;
    }
    
    for (int i = 0; i < max_lasers; i++) {
        if (!lasers[i].is_active()) {
            int active_count = get_alien_count();
            if (active_count == 0) return;
            
            int firing_alien = rand() % active_aliens;
            while (!aliens[firing_alien].active) {
                firing_alien = (firing_alien + 1) % active_aliens;
            }
            
            int alien_width = get_alien_width_for_type(aliens[firing_alien].type);
            int alien_height = get_alien_height_for_type(aliens[firing_alien].type);
            lasers[i].fire(
                aliens[firing_alien].x + alien_width / 2,
                aliens[firing_alien].y + alien_height
            );
            laser_timer.reset();
            break;
        }
    }
}

void Alien::update_lasers(Laser* lasers, int max_lasers) {
    for (int i = 0; i < max_lasers; i++) {
        if (lasers[i].is_active()) {
            lasers[i].update(false);
        }
    }
}