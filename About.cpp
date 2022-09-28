#include "About.h"

About::About() {
    bg = al_load_bitmap("./image/about/about_bg.png");
    abouttext = al_load_ttf_font("./font/pirulen.ttf", 60, 0);

    About_Text_width = al_get_bitmap_width(bg);
    About_Text_height = al_get_bitmap_height(bg);
}
About::~About(){
    al_destroy_bitmap(bg);
    al_destroy_font(abouttext);
}
void About::draw(){
    al_draw_bitmap(bg, 0, 0, 0);
    al_draw_text(abouttext, al_map_rgb(181, 223, 235), 180, 30, ALLEGRO_ALIGN_CENTRE, "ABOUT");
}

int About::process(ALLEGRO_EVENT *event) {
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            scene_change = 1;
            return MENU;
        }
    }
    return ABOUT;
}
