#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED
#include <map>
#include <vector>

#include "Barrier.h"
#include "Boss.h"
#include "Bullet.h"
#include "Missile.h"
#include "global.h"
class Character {
   public:
    // Construct a character with hp, atk, money, stage respectively.
    Character(int, int, int, int);
    // Construct a character with stage.
    Character(int);
    void process(ALLEGRO_EVENT *);
    void update_mouse(int, int);
    void update();
    void draw();
    void SaveGame();
    ~Character();

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_atk() { return atk; }
    int get_hp() { return hp; }
    int get_money() { return money; }
    int get_speed() { return speed; }
    int get_state() { return state; }
    int get_shield_time() { return shield_time; }
    bool get_game_state() { return gameover; }
    bool get_direction() { return dir; }
    void set_pos(int posx, int posy) {
        x = posx;
        y = posy;
    }
    void set_atk(int val) { atk = val; }
    void set_hp(int val) { hp = val; }
    void set_money(int val) { money = val; }
    void set_speed(int val) { speed = val; }
    void set_state(int val) { state = val; }
    void subtract_shield_time() { shield_time--; }
    void add_shield_time() { shield_time++; }

    void init_barrier(std::vector<Barrier *> *val) { bar = val; }
    void init_boss(Boss *val) { boss = val; }

   private:
    std::vector<ALLEGRO_BITMAP *> move;
    std::vector<ALLEGRO_BITMAP *> death;
    std::vector<ALLEGRO_BITMAP *> bullet_img;
    std::vector<ALLEGRO_BITMAP *> missile_img;
    std::vector<ALLEGRO_BITMAP *> shield_img;

    std::map<int, bool> keyboard;
    std::vector<Bullet *> bul;
    std::vector<Barrier *> *bar;
    std::vector<Missile *> mis;
    Boss *boss;
    int x, y;
    int width, height;
    bool dir;    // 0=right 1=left
    bool equip;  // 0=normal 1=equip
    bool skin;   // 0=normal
    bool auto_shooting;
    int state;  // 1=normal 2=hurt 3=death 4=no damage
    int stage_type;
    int atk_times;  // 50 = launch missile
    bool is_launch_missile;
    int missile_target_bar;

    int atk;
    int atk_cool_down;  // cool_down==speed => can shoot a bullet
    int atk_speed;
    int hp;
    int limit_hp;
    int money;
    int speed;
    int dx, dy;
    int shield_time;
    int shield_width, shield_height;

    int move_anime_time;
    int move_per_anime_time;
    int mouse_x, mouse_y;
    int hurt_no_damage_time;
    int death_anime_time;
    int death_anime_end_time;
    int create_new_missile_time;

    bool gameover;
};
#endif