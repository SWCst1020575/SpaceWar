#ifndef MISSILE_H_INCLUDED
#define MISSILE_H_INCLUDED

#include <vector>

#include "Barrier.h"
#include "Boss.h"
#include "global.h"
class Missile {
   public:
    Missile(std::vector<ALLEGRO_BITMAP *> *, int);
    Missile(int, std::vector<ALLEGRO_BITMAP *> *);
    void update(int, int, int, int, bool);
    void draw(bool);
    void SaveGame(int);
    ~Missile(){};

    void set_state(int val) { state = val; };
    void set_path(int val) { path_type = val; };
    void update_target_pos(Barrier *bar) { target_x = bar->get_x() + bar->get_width() / 2, target_y = bar->get_y() + bar->get_height() / 2; };
    void update_target_pos(Boss *boss) { target_x = boss->get_x() + boss->get_width() / 2, target_y = boss->get_y() + boss->get_height() / 2; };
    int get_x() { return (int)x; };
    int get_y() { return (int)y; };
    int get_width() { return width; };
    int get_height() { return height; };
    int get_damage() { return atk; }
    int get_state() { return state; };

    void init_speed() {
        switch (path_type) {
            case 0:
                dy = -speed;
                break;
            case 1:
                if (dir)
                    dx = -speed;
                else
                    dx = speed;
                break;
            case 2:
                dy = speed;
                break;
        }
    };

   private:
    std::vector<ALLEGRO_BITMAP *> *img;
    double x, y;
    double dx, dy;
    int width, height;
    int target_x, target_y;
    int speed;
    double degree;
    double now_degree;
    double target_degree;
    int state;  // 0=cycling 1=shooting 2=death 3=delete
    int path_type;
    bool dir;      // direction of Missile
    bool is_back;  // 0=behind character 1=in front of character

    int atk;

    int cycle_time;
    int flying_anime_time;
    int flying_anime_time_per;
    int quarter_cycle_time;
    int death_anime_end_time;
    int death_anime_time;
};

#endif