#include "Skin.h"

Skin::Skin() {
    bg = al_load_bitmap("./image/menu/menu_bg.png");
    //about_font = al_load_font("",24,0);
    text_color = new ALLEGRO_COLOR;
    *text_color = al_map_rgb(255, 255, 255);
}
int Skin::process(ALLEGRO_EVENT *event) {
    // TODO: Change skin, and save in data.
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            scene_change = 1;
            return MENU;
        }
    }
    return SKIN;
}
void Skin::draw() {
    al_draw_bitmap(bg, 0, 0, 0);
    
}
Skin::~Skin() {
    al_destroy_bitmap(bg);
    al_destroy_font(skin_font);
    delete text_color;
}