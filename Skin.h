#ifndef SKIN_H_INCLUDED
#define SKIN_H_INCLUDED
#include "global.h"
#include <string>
class Skin {
   public:
    Skin();
    int process(ALLEGRO_EVENT *);
    void draw();
    ~Skin();

   private:
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_FONT *skin_font = NULL;
    ALLEGRO_COLOR *text_color = NULL;
};
#endif