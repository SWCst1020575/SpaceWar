#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <string>

#include "global.h"
class Pause_Text {
   public:
    Pause_Text(std::string, int, int, int, int);
    void draw(ALLEGRO_FONT *, ALLEGRO_BITMAP *, int);
    void hover(int, int);
    bool get_state() { return state; };
    ~Pause_Text(){};

   private:
    int x, y;
    std::string text;
    int region_x, region_y, region_width, region_height;
    bool state;
};

class Pause {
   public:
    Pause();
    bool process(ALLEGRO_EVENT *);
    int get_is_scene_change(){return scene_change;};
    void text_state_check(int, int);
    void draw();
    ~Pause();

   private:
    ALLEGRO_FONT *pause_font = NULL;
    ALLEGRO_BITMAP *Menu_Text_hover = NULL;

    Pause_Text *return_game = nullptr;
    Pause_Text *return_menu = nullptr;
    Pause_Text *exit_game = nullptr;


    int Pause_Text_hover_width;
    int scene_change;//0=pause 1=menu 2=exit
    bool is_setting;
};
#endif