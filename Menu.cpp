#include "Menu.h"

#include "Config.h"

Menu::Menu() {
    bg = al_load_bitmap("./image/menu/menu_bg.png");
    menuFont = al_load_ttf_font("./font/pirulen.ttf", 60, 0);  // load font
    Menu_Text_hover = al_load_bitmap("./image/menu/menu_text_hover.png");
    start = new Menu_Text;
    about = new Menu_Text;
    setting = new Menu_Text;
    shop = new Menu_Text;
    skin = new Menu_Text;
    exit = new Menu_Text;
    Text_color = new ALLEGRO_COLOR;

    song = al_load_sample("./sound/menu.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(sample_instance, (float)config_read("music_vol") / 100.0);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_play_sample_instance(sample_instance);

    start->init("Start", window_width / 2, 80, 290, 70);
    about->init("About", window_width / 2, 190, 290, 70);
    setting->init("Setting", window_width / 2, 300, 370, 70);
    shop->init("Shop", window_width / 2, 410, 210, 70);
    skin->init("Skin", window_width / 2, 520, 210, 70);
    exit->init("Exit", window_width / 2, 630, 210, 70);
    *Text_color = al_map_rgb(181, 223, 235);

    Menu_Text_hover_width = al_get_bitmap_width(Menu_Text_hover);
    Menu_Text_hover_height = al_get_bitmap_height(Menu_Text_hover);
    stage_select = 0;
    select_stage1[0] = al_load_bitmap("./image/menu/stage1.png");
    select_stage1[1] = al_load_bitmap("./image/menu/stage1_hover.png");
    select_stage2[0] = al_load_bitmap("./image/menu/stage2.png");
    select_stage2[1] = al_load_bitmap("./image/menu/stage2_hover.png");
    select_stage3[0] = al_load_bitmap("./image/menu/stage3.png");
    select_stage3[1] = al_load_bitmap("./image/menu/stage3_hover.png");
    select_border = al_load_bitmap("./image/menu/stage_select_border.png");
    hexagon_width = al_get_bitmap_width(select_stage1[0]);
    hexagon_height = al_get_bitmap_height(select_stage1[0]);
}

Menu::~Menu() {
    al_destroy_bitmap(bg);
    al_destroy_font(menuFont);
    al_destroy_bitmap(Menu_Text_hover);
    al_destroy_bitmap(select_border);
    al_destroy_sample(song);
    al_detach_sample_instance(sample_instance);
    for (int i = 0; i <= 1; i++) {
        al_destroy_bitmap(select_stage1[i]);
        al_destroy_bitmap(select_stage2[i]);
        al_destroy_bitmap(select_stage3[i]);
    }
    delete start;
    delete about;
    delete setting;
    delete shop;
    delete skin;
    delete exit;
    delete Text_color;
}

void Menu::draw() {
    al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgb(0, 0, 0));
    al_draw_bitmap(bg, 0, 0, 0);

    start->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);
    about->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);
    setting->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);
    shop->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);
    skin->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);
    exit->draw(menuFont, *Text_color, Menu_Text_hover, Menu_Text_hover_width);

    if (stage_select) {
        al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba(0, 0, 0, 120));
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), window_width / 2, 20, ALLEGRO_ALIGN_CENTRE, select_str.c_str());

        if (select_stage1_state)
            al_draw_bitmap(select_stage1[1], (window_width - hexagon_width) / 2, 280 - hexagon_height / 2, 0);
        else
            al_draw_bitmap(select_stage1[0], (window_width - hexagon_width) / 2, 280 - hexagon_height / 2, 0);
        if (select_stage2_state)
            al_draw_bitmap(select_stage2[1], (window_width - hexagon_width) / 2 - 170, 580 - hexagon_height / 2, 0);
        else
            al_draw_bitmap(select_stage2[0], (window_width - hexagon_width) / 2 - 170, 580 - hexagon_height / 2, 0);
        if (select_stage3_state)
            al_draw_bitmap(select_stage3[1], (window_width - hexagon_width) / 2 + 170, 580 - hexagon_height / 2, 0);
        else
            al_draw_bitmap(select_stage3[0], (window_width - hexagon_width) / 2 + 170, 580 - hexagon_height / 2, 0);
        al_draw_bitmap(select_border, (window_width - hexagon_width) / 2, 280 - hexagon_height / 2, 0);
        al_draw_bitmap(select_border, (window_width - hexagon_width) / 2 - 170, 580 - hexagon_height / 2, 0);
        al_draw_bitmap(select_border, (window_width - hexagon_width) / 2 + 170, 580 - hexagon_height / 2, 0);
    }
}
int Menu::process(ALLEGRO_EVENT *event) {
    if (stage_select) {
        if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                stage_select = 0;
        } else if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (select_stage1_state) {
                stage_num = 1;
                scene_change = 1;
                return STAGE;
            } else if (select_stage2_state) {
                stage_num = 2;
                scene_change = 1;
                return STAGE;
            } else if (select_stage3_state) {
                stage_num = 3;
                scene_change = 1;
                return STAGE;
            }
        }
        return MENU;
    }
    if (event->type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (start->get_state()) {
            // scene_change = 1;
            // return STAGE;
            stage_select = 1;
            select_stage1_state = 0;
            select_stage2_state = 0;
            select_stage3_state = 0;
        } else if (about->get_state()) {
            scene_change = 1;
            return ABOUT;
        } else if (setting->get_state()) {
            scene_change = 1;
            return SETTING;
        } else if (shop->get_state()) {
            scene_change = 1;
            return SHOP;
        } else if (skin->get_state()) {
            scene_change = 1;
            return SKIN;
        } else if (exit->get_state()) {
            scene_change = 1;
            return EXIT_GAME;
        }
    }
    return MENU;
}
void Menu::text_state_check(int mouse_x, int mouse_y) {
    if (stage_select) {
        if ((mouse_x - window_width / 2) * (mouse_x - window_width / 2) + (mouse_y - 280) * (mouse_y - 280) < 172 * 172)
            select_stage1_state = 1;
        else
            select_stage1_state = 0;
        if ((mouse_x - window_width / 2 + 170) * (mouse_x - window_width / 2 + 170) + (mouse_y - 580) * (mouse_y - 580) < 172 * 172)
            select_stage2_state = 1;
        else
            select_stage2_state = 0;
        if ((mouse_x - window_width / 2 - 170) * (mouse_x - window_width / 2 - 170) + (mouse_y - 580) * (mouse_y - 580) < 172 * 172)
            select_stage3_state = 1;
        else
            select_stage3_state = 0;
        return;
    }
    start->hover(mouse_x, mouse_y);
    about->hover(mouse_x, mouse_y);
    setting->hover(mouse_x, mouse_y);
    shop->hover(mouse_x, mouse_y);
    skin->hover(mouse_x, mouse_y);
    exit->hover(mouse_x, mouse_y);
}

void Menu_Text::init(std::string s, int posx, int posy, int width, int height) {
    this->text = s;
    this->x = posx;
    this->y = posy;
    this->state = 0;
    this->region_x = posx - width / 2;
    this->region_y = posy;
    this->region_width = width;
    this->region_height = height;
}
void Menu_Text::draw(ALLEGRO_FONT *font_type, ALLEGRO_COLOR text_color, ALLEGRO_BITMAP *text_hover, int text_hover_width) {
    if (state)
        al_draw_bitmap(text_hover, x - text_hover_width / 2, y - 10, 0);
    al_draw_text(font_type, text_color, x, y, ALLEGRO_ALIGN_CENTRE, text.c_str());
}
void Menu_Text::hover(int mouse_x, int mouse_y) {
    if (mouse_x >= region_x && mouse_x <= (region_x + region_width) && mouse_y >= region_y && mouse_y <= (region_y + region_height))
        state = 1;
    else
        state = 0;
}