#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "global.h"
class Menu_Text {
   public:
    void init(std::string, int, int, int, int);
    void draw(ALLEGRO_FONT *, ALLEGRO_COLOR, ALLEGRO_BITMAP *, int);
    void hover(int, int);
    bool get_state() { return state; };
    ~Menu_Text(){};

   private:
    int x, y;
    std::string text;
    int region_x, region_y, region_width, region_height;
    bool state;
};

class Menu {
   public:
    Menu();
    ~Menu();

    // void Reset();
    int process(ALLEGRO_EVENT *);
    void draw();

    void text_state_check(int, int);
    int get_stage_num() { return stage_num; }

   private:
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_BITMAP *Menu_Text_hover = NULL;
    ALLEGRO_FONT *menuFont;
    ALLEGRO_COLOR *Text_color = NULL;
    ALLEGRO_SAMPLE *song = NULL;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;

    Menu_Text *start = NULL;
    Menu_Text *about = NULL;
    Menu_Text *setting = NULL;
    Menu_Text *shop = NULL;
    Menu_Text *skin = NULL;
    Menu_Text *exit = NULL;

    std::string select_str = "select stage";

    int Menu_Text_hover_width, Menu_Text_hover_height;
    bool stage_select;
    ALLEGRO_BITMAP *select_stage1[2];
    bool select_stage1_state;
    ALLEGRO_BITMAP *select_stage2[2];
    bool select_stage2_state;
    ALLEGRO_BITMAP *select_stage3[2];
    bool select_stage3_state;
    ALLEGRO_BITMAP *select_border = nullptr;
    int hexagon_width, hexagon_height;
    int stage_num;
};

#endif