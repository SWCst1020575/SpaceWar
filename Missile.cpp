#include "Missile.h"

#include <stdio.h>

#include <cmath>
#include <string>

#include "Config.h"
#define arc(a) (a * 3.14159265 / 180.0)
#define cycle_degree 90
#define cycle_r 55
Missile::Missile(std::vector<ALLEGRO_BITMAP *> *Missile_img, int atk_point) {
    img = Missile_img;
    state = 0;
    is_back = 0;
    atk = atk_point;
    speed = 20;
    cycle_time = 0;
    quarter_cycle_time = 21;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
    flying_anime_time = 0;
    flying_anime_time_per = 20;
    now_degree = 0;
    death_anime_end_time = 16;
    death_anime_time = 0;
}

void Missile::update(int char_x, int char_y, int char_width, int char_height, bool char_dir) {
    if (state == 0) {
        double t = (double)cycle_time / (double)quarter_cycle_time;
        dir = char_dir;
        int x_fix;
        if (dir)
            x_fix = -20;
        else
            x_fix = 10;
        if (is_back) {
            x = char_x + x_fix + char_width / 2 - cycle_r * cos(arc(cycle_degree)) + cycle_r * cos(arc(t * cycle_degree));
            y = char_y + char_height / 2 + cycle_r * sin(arc(t * cycle_degree));
            cycle_time--;
        } else {
            x = char_x + x_fix + char_width / 2 + cycle_r * cos(arc(cycle_degree)) - cycle_r * cos(arc(t * cycle_degree));
            y = char_y + char_height / 2 + cycle_r * sin(arc(t * cycle_degree));
            cycle_time++;
        }
        if (cycle_time == quarter_cycle_time || cycle_time == -quarter_cycle_time)
            is_back = !is_back;
    } else if (state == 1) {
        if (!dir)
            degree = -atan2(y + height / 2 - target_y, target_x - (x + width / 2));
        else
            degree = atan2(y + height / 2 - target_y, x + width / 2 - target_x);
        if (now_degree > degree + 0.05)
            now_degree -= 0.1;
        else if (now_degree < degree - 0.05)
            now_degree += 0.1;
        double x_distance = target_x - (x + width / 2);
        double y_distance = target_y - (y + height / 2);
        double target_dx = (double)speed * (x_distance / sqrt(x_distance * x_distance + y_distance * y_distance));
        double target_dy = (double)speed * (y_distance / sqrt(x_distance * x_distance + y_distance * y_distance));
        if (dx < target_dx - 0.25)
            dx += 0.5;
        else if (dx > target_dx + 0.25)
            dx -= 0.5;
        if (dy < target_dy - 0.25)
            dy += 0.5;
        else if (dy > target_dy + 0.25)
            dy -= 0.5;
        x += dx;
        y += dy;
    } else if (state == 2) {
        death_anime_time++;
        if (death_anime_time == death_anime_end_time)
            state = 3;
    }

    flying_anime_time++;
    if (flying_anime_time == flying_anime_time_per)
        flying_anime_time = 0;
    if (x + width < -800 || y + height < -800 || x > window_width + 800 || y > window_height + 800)
        state = 3;
}

void Missile::draw(bool back) {
    if (back == is_back) {
        switch (state) {
            case 0:
                if (flying_anime_time < flying_anime_time_per / 2)
                    al_draw_bitmap(img->at(0), x, y, dir);
                else
                    al_draw_bitmap(img->at(1), x, y, dir);
                break;
            case 1:
                if (flying_anime_time < flying_anime_time_per / 2)
                    al_draw_rotated_bitmap(img->at(0), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                else
                    al_draw_rotated_bitmap(img->at(1), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                break;
            case 2:
                if (death_anime_time < death_anime_end_time / 4)
                    al_draw_rotated_bitmap(img->at(2), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                else if (death_anime_time < death_anime_end_time / 4 * 2)
                    al_draw_rotated_bitmap(img->at(3), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                else if (death_anime_time < death_anime_end_time / 4 * 3)
                    al_draw_rotated_bitmap(img->at(4), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                else if (death_anime_time < death_anime_end_time)
                    al_draw_rotated_bitmap(img->at(5), width / 2, height / 2, x + width / 2, y + height / 2, now_degree, dir);
                break;
        }
    }
}
Missile::Missile(int num, std::vector<ALLEGRO_BITMAP *> *Missile_img) {
    std::string s = "Missile";
    s = s + std::to_string(num);
    img = Missile_img;
    state = config_read(s.c_str(), "STATE");
    is_back = config_read(s.c_str(), "IS_BACK");
    atk = config_read(s.c_str(), "ATK");
    speed = 20;
    cycle_time = config_read(s.c_str(), "CYCLE_TIME");
    quarter_cycle_time = 21;
    width = al_get_bitmap_width((*img)[0]);
    height = al_get_bitmap_height((*img)[0]);
    flying_anime_time = config_read(s.c_str(), "FLYING_ANIME_TIME");
    flying_anime_time_per = 20;
    now_degree = config_read_double(s.c_str(), "NOW_DEGREE");
    death_anime_end_time = 16;
    death_anime_time = config_read(s.c_str(), "DEATH_ANIME_TIME");
    x = config_read_double(s.c_str(), "X");
    y = config_read_double(s.c_str(), "Y");
    dx = config_read_double(s.c_str(), "DX");
    dy = config_read_double(s.c_str(), "DY");
    target_x = config_read(s.c_str(), "TARGET_X");
    target_y = config_read(s.c_str(), "TARGET_Y");
    degree = config_read_double(s.c_str(), "DEGREE");
    target_degree = config_read_double(s.c_str(), "TARGET_DEGREE");
    path_type = config_read(s.c_str(), "PATH_TYPE");
    dir = config_read(s.c_str(), "DIRECTION");
}
void Missile::SaveGame(int num) {
    std::string s = "Missile";
    s = s + std::to_string(num);
    config_write(s.c_str(), "X", x);
    config_write(s.c_str(), "Y", y);
    config_write(s.c_str(), "DX", dx);
    config_write(s.c_str(), "DY", dy);
    config_write(s.c_str(), "STATE", state);
    config_write(s.c_str(), "ATK", atk);
    config_write(s.c_str(), "DEATH_ANIME_TIME", death_anime_time);
    config_write(s.c_str(), "TARGET_X", target_x);
    config_write(s.c_str(), "TARGET_Y", target_y);
    config_write(s.c_str(), "DEGREE", degree);
    config_write(s.c_str(), "NOW_DEGREE", now_degree);
    config_write(s.c_str(), "TARGET_DEGREE", target_degree);
    config_write(s.c_str(), "PATH_TYPE", path_type);
    config_write(s.c_str(), "DIRECTION", (int)dir);
    config_write(s.c_str(), "IS_BACK", (int)is_back);
    config_write(s.c_str(), "CYCLE_TIME", cycle_time);
    config_write(s.c_str(), "FLYING_ANIME_TIME", flying_anime_time);
}