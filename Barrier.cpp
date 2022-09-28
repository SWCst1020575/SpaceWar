#include "Barrier.h"

#include <string>

#include "Config.h"
Barrier::Barrier(int difficulty, int level, int bar_type, ALLEGRO_BITMAP* image) {
    double enhance;
    lv = level;
    if (difficulty == 1)
        enhance = 1;
    else if (difficulty == 2)
        enhance = 1.5;
    else if (difficulty == 3)
        enhance = 2;
    img = image;
    height = al_get_bitmap_height(img);
    width = al_get_bitmap_width(img);
    x = window_width;
    y = rand() % (window_height - height);
    hp = ((80 + lv * 20) * 0.9 + rand() % (int)(100 * 0.2)) * enhance;
    atk = (70 + lv * 10) * enhance;
    money = (100 * 0.9 + rand() % (int)(100 * 0.2)) * enhance;
    speed = 6;
    dx = -speed;
    dy = 0;
    type = bar_type;
    state = 1;
    death_anime_time = 0;
    death_anime_end_time = 12;
    is_split = 0;

    shade_time = FPS * 3;
    shade_time_count = 0;
    appear_time = FPS;
    shade_anime_time = FPS / 3;
    split_cool_down = 0;
}
void Barrier::update() {
    // TODO: Detect collision with character and compute the damage.
    // TODO: type 2, type 3 barrier.
    if (type == 3) {
        shade_time_count++;
        if (shade_time_count == (appear_time + shade_time))
            shade_time_count = 0;
    }

    if (state == 1) {
        switch (type) {
            case 0:
            case 1:
            case 2:
            case 3:
                x += dx;
                y += dy;
                if (dy >= 1)
                    dy -= 1;
                else if (dy <= -1)
                    dy += 1;
                else
                    dy = 0;
        }
        if (x + width < 0)
            state = 3;
    } else if (state == 2) {
        death_anime_time++;
        if (death_anime_time == death_anime_end_time)
            state = 3;
    }
    if (hp < 0 && state == 1)
        state = 2;
    if (split_cool_down == 0)
        is_split = 0;
    else if (split_cool_down > 0)
        split_cool_down--;
}
void Barrier::draw() {
    if (state == 1) {
        if (type == 0 || type == 1 || type == 2)
            al_draw_bitmap(img, x, y, 0);
        else if (type == 3) {
            if (shade_time_count < appear_time)
                al_draw_bitmap(img, x, y, 0);
            else if (shade_time_count <= appear_time + shade_anime_time) {
                int color_ = 255 * (shade_anime_time + appear_time - shade_time_count) / shade_anime_time;
                al_draw_tinted_bitmap(img, al_map_rgba(color_, color_, color_, color_), x, y, 0);
            } else if (shade_time_count >= appear_time + shade_time - shade_anime_time)
                al_draw_tinted_bitmap(img, al_map_rgba(255, 255, 255, 255 * (shade_time_count - (appear_time + shade_time - shade_anime_time)) / shade_anime_time), x, y, 0);
        }
    } else if (state == 2)
        al_draw_tinted_bitmap(img, al_map_rgba(255, 255, 255, 255 * death_anime_time / death_anime_end_time), x, y, 0);
}
Barrier::Barrier(Barrier* tar) {
    img = tar->img;
    x = tar->x;
    y = tar->y;
    dx = tar->dx;
    dy = tar->dy;
    width = tar->width;
    height = tar->height;
    type = tar->type;
    state = tar->state;
    is_split = tar->is_split;

    shade_time = tar->shade_time;
    shade_time_count = tar->shade_time_count;
    shade_anime_time = tar->shade_anime_time;
    appear_time = tar->appear_time;
    split_cool_down = tar->split_cool_down;

    atk = tar->atk;
    hp = tar->hp;
    money = tar->money;
    speed = tar->speed;

    death_anime_time = tar->death_anime_time;
    death_anime_end_time = tar->death_anime_end_time;
}
Barrier::Barrier(int num, std::vector<ALLEGRO_BITMAP*>* image) {
    std::string s = "Barrier";
    s = s + std::to_string(num);
    lv = config_read(s.c_str(), "LV");
    type = config_read(s.c_str(), "TYPE");
    img = (*image)[type];
    height = al_get_bitmap_height(img);
    width = al_get_bitmap_width(img);
    x = config_read(s.c_str(), "X");
    y = config_read(s.c_str(), "Y");
    hp = config_read(s.c_str(), "HP");
    atk = config_read(s.c_str(), "ATK");
    money = config_read(s.c_str(), "MONEY");
    speed = 6;
    dx = config_read(s.c_str(), "DX");
    dy = config_read(s.c_str(), "DY");
    state = config_read(s.c_str(), "STATE");
    death_anime_time = config_read(s.c_str(), "DEATH_ANIME_TIME");
    death_anime_end_time = 12;
    is_split = config_read(s.c_str(), "IS_SPLIT");

    shade_time = FPS * 3;
    shade_time_count = config_read(s.c_str(), "SHADE_TIME_COUNT");
    appear_time = FPS;
    shade_anime_time = FPS / 3;
    split_cool_down = config_read(s.c_str(), "SPLIT_COOL_DOWN");
}
void Barrier::SaveGame(int num) {
    std::string s = "Barrier";
    s = s + std::to_string(num);
    config_write(s.c_str(), "X", x);
    config_write(s.c_str(), "Y", y);
    config_write(s.c_str(), "DX", dx);
    config_write(s.c_str(), "DY", dy);
    config_write(s.c_str(), "TYPE", type);
    config_write(s.c_str(), "STATE", state);
    config_write(s.c_str(), "IS_SPLIT", is_split);
    config_write(s.c_str(), "SHADE_TIME_COUNT", shade_time_count);
    config_write(s.c_str(), "SPLIT_COOL_DOWN", split_cool_down);
    config_write(s.c_str(), "LV", lv);
    config_write(s.c_str(), "ATK", atk);
    config_write(s.c_str(), "HP", hp);
    config_write(s.c_str(), "MONEY", money);
    config_write(s.c_str(), "DEATH_ANIME_TIME", death_anime_time);
}