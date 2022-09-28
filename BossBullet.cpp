#include "BossBullet.h"

#include <cmath>
#include <string>

#include "Config.h"
#define arc(a) (a * 3.14159265 / 180.0)
BossBullet::BossBullet(int pos_x, int pos_y, int atk, int deg, std::vector<ALLEGRO_BITMAP *> *bul_img) {
    x = pos_x;
    y = pos_y;
    damage = atk;
    img = bul_img;
    speed = 10;
    degree = deg;
    dx = (double)speed * cos(arc(degree));
    dy = (double)speed * (-sin(arc(degree)));
    inertia_time = 10;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
    death_anime_end_time = 18;
    death_anime_time = 0;
    state = 1;
    appear_time = FPS;
}
void BossBullet::update() {
    if (state == 1) {
        if (speed > 5)
            speed -= 0.5;
        dx = (double)speed * cos(arc(degree));
        dy = (double)speed * (-sin(arc(degree)));
        inertia_x = dy * ((double)inertia_time / 10.0);
        inertia_y = dx * ((double)inertia_time / 10.0);
        x += (dx + inertia_x);
        y += (dy + inertia_y);
        if (inertia_time > 0)
            inertia_time--;
    } else if (state == 2) {
        death_anime_time++;
        if (death_anime_time == death_anime_end_time)
            state = 3;
    }
    if (appear_time > 0)
        appear_time--;
    if (x + width < 0 || x > window_width || y + height < 0 || y > window_height)
        state = 3;
}
void BossBullet::draw() {
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
void BossBullet::blind_draw() {
    if (state == 1) {
        int color = (255 * (double(appear_time) / FPS));
        al_draw_tinted_bitmap((*img)[0], al_map_rgba(color, color, color, color), (int)x, (int)y, 0);
    } else if (state == 2) {
        if (death_anime_time < death_anime_end_time / 3)
            al_draw_bitmap((*img)[1], (int)x, (int)y, 0);
        else if (death_anime_time < death_anime_end_time * 2 / 3)
            al_draw_tinted_bitmap((*img)[2], al_map_rgba(255, 255, 255, 200), (int)x, (int)y, 0);
        else
            al_draw_tinted_bitmap((*img)[3], al_map_rgba(255, 255, 255, 150), (int)x, (int)y, 0);
    }
}
BossBullet::BossBullet(int num, std::vector<ALLEGRO_BITMAP *> *bul_img) {
    std::string s = "BossBullet";
    s = s + std::to_string(num);
    x = config_read_double(s.c_str(), "X");
    y = config_read_double(s.c_str(), "Y");
    damage = config_read(s.c_str(), "DAMAGE");
    img = bul_img;
    speed = 10;
    degree = config_read(s.c_str(), "DEGREE");
    dx = config_read_double(s.c_str(), "DX");
    dy = config_read_double(s.c_str(), "DY");
    inertia_time = 10;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
    death_anime_end_time = 18;
    death_anime_time = config_read(s.c_str(), "DEATH_ANIME_TIME");
    state = config_read(s.c_str(), "STATE");
    appear_time = config_read(s.c_str(), "APPEAR_TIME");
    inertia_x = config_read_double(s.c_str(), "INERTIA_X");
    inertia_y = config_read_double(s.c_str(), "INERTIA_Y");
}
void BossBullet::SaveGame(int num) {
    std::string s = "BossBullet";
    s = s + std::to_string(num);
    config_write(s.c_str(), "X", x);
    config_write(s.c_str(), "Y", y);
    config_write(s.c_str(), "DX", dx);
    config_write(s.c_str(), "DY", dy);
    config_write(s.c_str(), "STATE", state);
    config_write(s.c_str(), "DAMAGE", damage);
    config_write(s.c_str(), "INERTIA_X", inertia_x);
    config_write(s.c_str(), "INERTIA_Y", inertia_y);
    config_write(s.c_str(), "DEGREE", degree);
    config_write(s.c_str(), "DEATH_ANIME_TIME", death_anime_time);
    config_write(s.c_str(), "APPEAR_TIME", appear_time);
}