#include "Pause.h"

#include "Config.h"
Pause::Pause() {
    // about_font = al_load_font("",24,0);

    // Pause_Text_hover_width = al_get_bitmap_width(Menu_Text_hover);
    pause_font = al_load_ttf_font("./font/pirulen.ttf", 60, 0);
    return_game = new Pause_Text("return", window_width / 2, 140, 330, 70);
    return_menu = new Pause_Text("menu", window_width / 2, 340, 210, 70);
    exit_game = new Pause_Text("exit", window_width / 2, 540, 210, 70);
    scene_change = 0;
    is_setting = 0;
}
bool Pause::process(ALLEGRO_EVENT *event) {
    // TODO: If press esc in stage, call pause.
    // TODO: Pause menu: return to game, shop,back to menu.
    // TODO: When calling pause, save a data(stage_num, character status, enemy status).
    //       If close game, the status of game will be saved.
    if (event->type == ALLEGRO_EVENT_KEY_DOWN)
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            return 0;
    if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (return_game->get_state())
            return 0;
        else if (return_menu->get_state()) {
            scene_change = 1;
            config_write("Stage", "GAME_CONTINUE", 0);
            config_write("missle", 0);
            return 0;
        } else if (exit_game->get_state()) {
            scene_change = 2;
            return 0;
        }
    }
    return 1;
}
void Pause::draw() {
    al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba(100, 100, 100, 80));
    // al_draw_text(about_font, *text_color, window_width / 2, 20, 0, str.c_str());
    return_game->draw(pause_font, Menu_Text_hover, Pause_Text_hover_width);
    return_menu->draw(pause_font, Menu_Text_hover, Pause_Text_hover_width);
    exit_game->draw(pause_font, Menu_Text_hover, Pause_Text_hover_width);
}
void Pause::text_state_check(int mouse_x, int mouse_y) {
    return_game->hover(mouse_x, mouse_y);
    return_menu->hover(mouse_x, mouse_y);
    exit_game->hover(mouse_x, mouse_y);
}

Pause::~Pause() {
    al_destroy_font(pause_font);
    al_destroy_bitmap(Menu_Text_hover);

    delete return_game;
    delete return_menu;
    delete exit_game;
}

Pause_Text::Pause_Text(std::string s, int posx, int posy, int width, int height) {
    text = s;
    x = posx;
    y = posy;
    state = 0;
    region_x = posx - width / 2;
    region_y = posy;
    region_width = width;
    region_height = height;
}
void Pause_Text::draw(ALLEGRO_FONT *font_type, ALLEGRO_BITMAP *text_hover, int text_hover_width) {
    if (state)
        al_draw_text(font_type, al_map_rgb(0, 0, 150), x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
    else
        al_draw_text(font_type, al_map_rgb(255, 255, 255), x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
}
void Pause_Text::hover(int mouse_x, int mouse_y) {
    if (mouse_x >= region_x && mouse_x <= (region_x + region_width) && mouse_y >= region_y && mouse_y <= (region_y + region_height))
        state = 1;
    else
        state = 0;
}