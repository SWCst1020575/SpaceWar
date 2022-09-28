#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED
#include <vector>

#include "global.h"
class Bullet {
   public:
    // Construct a character with x, y, x_mouse, y_mouse, damage, speed, image respectively.
    Bullet(int, int, int, int, int, int, std::vector<ALLEGRO_BITMAP *> *);
    Bullet(int, std::vector<ALLEGRO_BITMAP *> *);
    void update();
    void draw();
    void SaveGame(int);
    ~Bullet(){};

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

    int death_anime_end_time;
    int death_anime_time;
};
#endif