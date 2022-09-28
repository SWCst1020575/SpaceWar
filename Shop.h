#ifndef SHOP_H_INCLUDED
#define SHOP_H_INCLUDED
#include "global.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "global.h"
#include "Config.h"
#include <iostream>
#include <string>

#define SKIN_PRICE 10
#define MISSLE_PRICE 10
#define SHIELD_PRICE 10
#define ATK_PRICE 5
#define HP_PRICE 5

#define SHIELD_MAX 4
#define ATK_MAX 100
#define HP_MAX 500

#define SHIELD_UNIT 1
#define ATK_UNIT 10
#define HP_UNIT 10


class Chara_skin{
public:
    void init(const char*);
    void draw(int, int); //dx, dy
    void hover(int, int);
    void press(){click = true;}
    void release(){click = false;}
    ~Chara_skin() {}
private:
    int skin_now, avail, x, y, region_width, region_height, coin;
    std::string option;
    ALLEGRO_BITMAP * skin[4] = {NULL};
    ALLEGRO_BITMAP * skin_grey = NULL;
    ALLEGRO_FONT *buy_font = NULL;
    int move_anime_time;
    int move_per_anime_time;
    bool state = 0;
    bool click = 0;
};
class Missle{
public:
    void init(const char*);
    void draw(int, int);
    void hover(int, int);
    void press(){click = true;}
    void release(){click = false;}
    ~Missle() {}
private:
    int skin_now, avail, x, y, region_width, region_height, coin;
    std::string option;
    ALLEGRO_BITMAP * skin1_grey = NULL;
    ALLEGRO_BITMAP * skin2_grey = NULL;
    ALLEGRO_BITMAP * skin1[4] = {NULL};
    ALLEGRO_BITMAP * skin2[4] = {NULL};
    ALLEGRO_FONT *buy_font = NULL;
    int move_anime_time;
    int move_per_anime_time;
    bool state = 0;
    bool click = 0;
};

class Container{
    public:
    void init(const char*);
    void draw(int, int);
    void hover(int, int);
    void press(){click = true;}
    void release(){click = false;}
    ~Container() {}
private:
    int quan, x, y, region_width, region_height;
    int dis1 = 150;
    int dis2 = 200;
    std::string option;
    ALLEGRO_BITMAP * item = NULL;
    ALLEGRO_BITMAP * sub = NULL;
    ALLEGRO_BITMAP * add = NULL;
    ALLEGRO_FONT *quan_font = NULL;
    int move_anime_time;
    int move_per_anime_time;
    bool state_sub = 0;
    bool state_add = 0;
    bool click = 0;
    bool flag;
};

class Shop {
   public:
    Shop();
    int process(ALLEGRO_EVENT *);
    void draw();
    void press();
    void release();
    void button_state_check(int, int);
    ~Shop();

   private:
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_BITMAP *coin = NULL;
    ALLEGRO_FONT *shop_font = NULL;
    ALLEGRO_FONT *money = NULL;
    Chara_skin* skin1;
    Chara_skin* skin2;
    Missle* missle;
    Container* shield;
    Container* atk;
    Container* hp;

};
#endif
