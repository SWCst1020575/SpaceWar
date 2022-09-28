#ifndef ABOUT_H_INCLUDED
#define ABOUT_H_INCLUDED
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "global.h"
#include <string>


class About {
   public:
    About();
    int process(ALLEGRO_EVENT *);
    void draw();
    ~About();

   private:
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_FONT  *abouttext = NULL;

    int About_Text_width, About_Text_height;
};
#endif
