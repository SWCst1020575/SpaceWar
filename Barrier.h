#ifndef BARRIER_H_INCLUDED
#define BARRIER_H_INCLUDED
#include <vector>

#include "global.h"
class Barrier {
   public:
    // Construct a barrier with difficulty, level, barrier_type respectively.
    Barrier(int, int, int, ALLEGRO_BITMAP *);
    Barrier(int, std::vector<ALLEGRO_BITMAP *> *);
    // Copy
    Barrier(Barrier *);
    void update();
    void draw();
    void SaveGame(int);
    ~Barrier(){};

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_atk() { return atk; }
    int get_hp() { return hp; }
    int get_money() { return money; }
    int get_speed() { return speed; }
    int get_state() { return state; }
    int get_type() { return type; }
    bool get_split() { return is_split; }
    void set_pos(int posx, int posy) {
        x = posx;
        y = posy;
    }
    void set_atk(int val) { atk = val; }
    void set_hp(int val) { hp = val; }
    void set_money(int val) { money = val; }
    void set_speed(int val) { speed = val; }
    void set_x_speed(int val) { dx = val; }
    void set_y_speed(int val) { dy = val; }
    void set_shade_time_count(int val) { shade_time_count = val; }
    void set_split_cool_down(int val) { split_cool_down = val; }
    void set_split(bool val) { is_split = val; }

   private:
    ALLEGRO_BITMAP *img;
    int x, y;
    int dx, dy;
    int width, height;
    int type;   // 0,1=normal barrier / 2=split barrier / 3=invisible barrier
    int state;  // 1=flying 2=collapse
    bool is_split;

    int shade_time;
    int shade_time_count;
    int shade_anime_time;
    int split_cool_down;
    int appear_time;

    int lv;
    int atk;
    int hp;
    int money;
    int speed;

    int death_anime_time;
    int death_anime_end_time;
};
#endif