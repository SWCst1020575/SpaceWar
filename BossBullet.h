#ifndef BOSSBULLET_H_INCLUDED
#define BOSSBULLET_H_INCLUDED
#include <vector>

#include "global.h"

class BossBullet {
   public:
    // Construct a character with x, y, damage, degree, image respectively.
    BossBullet(int, int, int, int, std::vector<ALLEGRO_BITMAP *> *);
    BossBullet(int, std::vector<ALLEGRO_BITMAP *> *);
    void update();
    void draw();
    void blind_draw();
    void SaveGame(int);
    ~BossBullet(){};

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_damage() { return damage; }
    int get_state() { return state; }
    void set_damage(int val) { damage = val; }
    void set_state(int val) { state = val; }

   private:
    std::vector<ALLEGRO_BITMAP *> *img;

    double x, y;
    int width, height;
    int state;  // 1=flying 2=death 3=delete

    int damage;
    double speed;
    double dx, dy;
    double inertia_x, inertia_y;
    int degree;

    int death_anime_end_time;
    int death_anime_time;
    int inertia_time;

    int appear_time;
};

#endif