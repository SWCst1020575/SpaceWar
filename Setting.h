#ifndef SETTING_H_INCLUDED
#define SETTING_H_INCLUDED
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "global.h"
#include "Config.h"
#include <string>
#include <iostream>


#include "global.h"

class Setting_option{
public:
    void init(const char*);
    void draw(int);
    void hover(int, int);
    void press(int);
    void release(){click=false;}
    bool get_state() {return state;}
    void save();
    ~Setting_option() {delete this;}

private:
    int x, y, val;
    int dis;
    int region_x, region_y, region_width, region_height;
    ALLEGRO_BITMAP *shape_white = NULL;
    ALLEGRO_BITMAP *shape_blue = NULL;
    ALLEGRO_FONT  *numtext = NULL;
    int move_anime_time;
    int move_per_anime_time;

    std::string option;
    bool state = 0;
    bool click = 0;
    bool flag = 0;
};

class Setting {
   public:
    Setting();
    int process(ALLEGRO_EVENT *);
    void draw();
    void press();
    void release();
    void option_state_check(int, int);
    ~Setting();

   private:
    int mouse_x, mouse_y;
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_FONT *settingtext = NULL;
    ALLEGRO_COLOR *text_color = NULL;

    Setting_option *music_vol;
    Setting_option *sound_effect_vol;
    Setting_option *automation;
    Setting_option *difficulty;
};


#endif
