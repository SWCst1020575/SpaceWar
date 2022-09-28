#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include <allegro5/allegro_ttf.h>

#include <vector>

#include "Boss.h"
#include "Character.h"
#include "global.h"
class Interface {
   public:
    Interface();
    void draw(Character *);
    void draw(Character *, Boss *);
    ~Interface();


   private:
    ALLEGRO_BITMAP *hp_img = NULL;
    ALLEGRO_BITMAP *coin_bar_img = NULL;
    ALLEGRO_BITMAP *boss_hp_bar_img = NULL;
    ALLEGRO_BITMAP *boss_hpfull_bar_img = NULL;
    ALLEGRO_FONT *interfaceFont = NULL;
    int hp_img_width, hp_img_height;
    int coin_bar_img_width, coin_bar_img_height;
    int boss_hp_bar_width;
    int boss_hpfull_bar_height,boss_hpfull_bar_width;

    int hp;
    int enemy_hp;
    int now_enemy_hp;

    int start_boss_hp_bar_anime;
};
#endif