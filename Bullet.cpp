#include "Bullet.h"

#include <cmath>
#include <string>

#include "Config.h"
Bullet::Bullet(int pos_x, int pos_y, int mouse_x, int mouse_y, int bullet_dmg, int bullet_speed, std::vector<ALLEGRO_BITMAP *> *image) {
    x = pos_x;
    y = pos_y;
    double x_to_mousex = (double)mouse_x - x, y_to_mousey = (double)mouse_y - y;
    double r = sqrt(x_to_mousex * x_to_mousex + y_to_mousey * y_to_mousey);
    speed = bullet_speed;
    damage = bullet_dmg;
    dx = speed * (x_to_mousex / r);
    dy = speed * (y_to_mousey / r);
    death_anime_end_time = 18;
    death_anime_time = 0;

    state = 1;
    img = image;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
}
void Bullet::update() {
    // TODO: Update the position of bullet.
    // TODO: Detect collision with barrier and compute the damage.
    if (state == 1) {
        x += dx;
        y += dy;
    } else if (state == 2) {
        death_anime_time++;
        if (death_anime_time == death_anime_end_time)
            state = 3;
    }
    if (x + width < 0 || x > window_width || y + height < 0 || y > window_height)
        state = 3;
}
void Bullet::draw() {
    // al_draw_bitmap(, x, y, 0);
    if (state == 1)
        al_draw_bitmap((*img)[0], (int)x, (int)y, 0);
    else if (state == 2) {
        if (death_anime_time < death_anime_end_time / 3)
            al_draw_bitmap((*img)[1], (int)x, (int)y, 0);
        else if (death_anime_time < death_anime_end_time * 2 / 3)
            al_draw_tinted_bitmap((*img)[2], al_map_rgba(255, 255, 255, 200), (int)x, (int)y, 0);
        else
            al_draw_tinted_bitmap((*img)[3], al_map_rgba(255, 255, 255, 150), (int)x, (int)y, 0);
    }
}
Bullet::Bullet(int num, std::vector<ALLEGRO_BITMAP *> *image) {
    std::string s = "Bullet";
    s = s + std::to_string(num);
    x = config_read_double(s.c_str(), "X");
    y = config_read_double(s.c_str(), "Y");
    
    speed = config_read_double(s.c_str(), "SPEED");
    damage = config_read(s.c_str(), "DAMAGE");
    dx = config_read_double(s.c_str(), "DX");
    dy = config_read_double(s.c_str(), "DY");
    death_anime_end_time = 18;
    death_anime_time = config_read(s.c_str(), "DEATH_ANIME_TIME");

    state = config_read(s.c_str(), "STATE");
    img = image;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
}
void Bullet::SaveGame(int num) {
    std::string s = "Bullet";
    s = s + std::to_string(num);
    config_write(s.c_str(), "X", x);
    config_write(s.c_str(), "Y", y);
    config_write(s.c_str(), "DX", dx);
    config_write(s.c_str(), "DY", dy);
    config_write(s.c_str(), "STATE", state);
    config_write(s.c_str(), "DAMAGE", damage);
    config_write(s.c_str(), "SPEED", speed);
    config_write(s.c_str(), "DEATH_ANIME_TIME", death_anime_time);
}