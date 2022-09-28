#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


#include "global.h"
class Item {
   public:
    // Construct a item with item_type.
    Item(int, ALLEGRO_BITMAP *);
    void update();
    void draw();
    void blind_draw();
    ~Item(){};

    int get_x() { return x; }
    int get_y() { return y; }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_hp() { return hp; }
    int get_money() { return money; }
    int get_speed() { return speed; }
    int get_state() { return state; }
    int get_type() { return type; }
    void set_pos(int posx, int posy) {
        x = posx;
        y = posy;
    }
    void set_hp(int val) { hp = val; }
    void set_money(int val) { money = val; }
    void set_speed(int val) { speed = val; }
    void set_x_speed(int val) { dx = val; }
    void set_y_speed(int val) { dy = val; }
    void set_state(int val) { state = val; }

   private:
    ALLEGRO_BITMAP *img;
    int x, y;
    int dx, dy;
    int width, height;
    int type;   //0=heart
    int state;  // 1=flying 2=delete

    int hp;
    int money;
    int speed;

    int death_anime_time;
    int death_anime_end_time;

    int blind_time;
    int blind_dt;
};
#endif