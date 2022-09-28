#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include <allegro5/allegro_primitives.h>

#include <vector>

#include "Barrier.h"
#include "BossBullet.h"
#include "Item.h"
#include "global.h"
class Boss {
   public:
    // Construct a character with hp, atk, money, speed respectively.
    Boss(int);
    Boss();
    void process(ALLEGRO_EVENT *);
    void update();
    void draw();
    void draw_blind();
    void SaveGame();
    ~Boss();

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_weakness_x() { return (int)weakness_x; }
    int get_weakness_y() { return (int)weakness_y; }
    int get_weakness_width() { return weakness_width; }
    int get_weakness_height() { return weakness_height; }
    int get_atk() { return atk; }
    int get_hp() { return hp; }
    int get_full_hp() { return full_hp; }
    int get_money() { return money; }
    int get_speed() { return speed; }
    int get_state() { return state; }
    std::vector<BossBullet *> *get_bossbullet_pointer() { return &bul; }
    bool get_is_weak() { return is_weak; }
    bool get_game_state() { return pass_game; }
    void set_pos(int posx, int posy) {
        x = posx;
        y = posy;
    }
    void set_atk(int val) { atk = val; }
    void set_hp(int val) { hp = val; }
    void set_full_hp(int val) { full_hp = val; }
    void set_money(int val) { money = val; }
    void set_speed(int val) { speed = val; }
    void set_state(int val) { state = val; }

    void update_blind_pos(int pos_x, int pos_y) {
        if (is_blind)
            blind_x = pos_x - blind_width / 2, blind_y = pos_y - blind_height / 2;
    }
    void init_itm(std::vector<Item *> *val) { itm = val; }

   private:
    std::vector<ALLEGRO_BITMAP *> move;
    std::vector<ALLEGRO_BITMAP *> death;
    std::vector<ALLEGRO_BITMAP *> weakness;
    std::vector<ALLEGRO_BITMAP *> bullet_img;

    std::vector<BossBullet *> bul;
    ALLEGRO_BITMAP *blind_img = NULL;
    std::vector<Item *> *itm;
    int blind_x, blind_y;
    int blind_width, blind_height;

    double x, y;
    int width, height;
    bool dir;   // 0=right 1=left
    int state;  // 1=stop 2=move 3=death
    int lv;     // level of boss

    bool is_shooting;     // use bullet attack
    bool is_blind;        // blind character
    bool is_weak;

    int atk;
    int atk_cool_down;  // cool_down==speed => can shoot a bullet
    int full_hp;
    int hp;
    int money;
    int speed;
    double dx, dy;
    int destination_x, destination_y;
    int weakness_x, weakness_y;
    double weakness_distance_x, weakness_distance_y;
    int weakness_width, weakness_height;

    int change_pos_time;    //=0 move
    int weakness_time;      // change weakness position 10s
    int weakness_cooldown;  // 5s
    int move_time;
    int death_anime_time;
    int death_anime_end_time;
    int move_anime_time;
    int move_per_anime_time;
    int bullet_time;
    int bullet_time_end;
    int bullet_degree;
    int bullet_pattern;

    int blind_time;

    bool pass_game;
};
#endif