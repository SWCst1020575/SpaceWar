#include "Interface.h"

#include <cmath>
#include <string>
Interface::Interface() {
    hp_img = al_load_bitmap("./image/items/item2.png");
    coin_bar_img = al_load_bitmap("./image/interface/money_bar.png");
    boss_hp_bar_img = al_load_bitmap("./image/interface/boss_hp_bar.png");
    boss_hpfull_bar_img = al_load_bitmap("./image/interface/boss_hp_bar_full.png");
    interfaceFont = al_load_ttf_font("./font/pirulen.ttf", 24, 0);
    hp_img_width = al_get_bitmap_width(hp_img);
    hp_img_height = al_get_bitmap_height(hp_img);
    coin_bar_img_width = al_get_bitmap_width(coin_bar_img);
    coin_bar_img_height = al_get_bitmap_height(coin_bar_img);
    boss_hp_bar_width = al_get_bitmap_width(boss_hp_bar_img);
    boss_hpfull_bar_height = al_get_bitmap_height(boss_hpfull_bar_img);
    boss_hpfull_bar_width = al_get_bitmap_width(boss_hpfull_bar_img);

    start_boss_hp_bar_anime = 0;
}
void Interface::draw(Character *cha) {
    // TODO: Draw hp, money ... .

    hp = cha->get_hp();
    int heart_num = ceil((double)hp / (double)100);
    for (int i = 0; i < heart_num; i++) {
        if (i == heart_num - 1)
            al_draw_bitmap_region(hp_img, 0, 0, hp_img_width * ((double)(hp - i * 100) / (double)100), hp_img_height, window_width / 2 + hp_img_width * (i - heart_num / 2) + 20 * (i - (heart_num - 1) / 2), window_height - hp_img_height - 10, 0);
        else
            al_draw_bitmap(hp_img, window_width / 2 + hp_img_width * (i - heart_num / 2) + 20 * (i - (heart_num - 1) / 2), window_height - hp_img_height - 10, 0);
    }
    al_draw_bitmap(coin_bar_img, 20, 20, 0);
    al_draw_text(interfaceFont, al_map_rgb(255, 255, 255), 20 + coin_bar_img_width - 45, 29, ALLEGRO_ALIGN_RIGHT, std::to_string(cha->get_money()).c_str());
}
void Interface::draw(Character *cha, Boss *boss) {
    // TODO: Draw hp, money ... .

    hp = cha->get_hp();
    int heart_num = ceil((double)hp / (double)100);
    for (int i = 0; i < heart_num; i++) {
        if (i == heart_num - 1)
            al_draw_bitmap_region(hp_img, 0, 0, hp_img_width * ((double)(hp - i * 100) / (double)100), hp_img_height, window_width / 2 + hp_img_width * (i - heart_num / 2) + 20 * (i - (heart_num - 1) / 2), window_height - hp_img_height - 10, 0);
        else
            al_draw_bitmap(hp_img, window_width / 2 + hp_img_width * (i - heart_num / 2) + 20 * (i - (heart_num - 1) / 2), window_height - hp_img_height - 10, 0);
    }
    al_draw_bitmap(coin_bar_img, 20, 20, 0);
    al_draw_text(interfaceFont, al_map_rgb(255, 255, 255), 20 + coin_bar_img_width - 45, 29, ALLEGRO_ALIGN_RIGHT, std::to_string(cha->get_money()).c_str());
    al_draw_bitmap(boss_hp_bar_img, window_width - boss_hp_bar_width - 40, 20, 0);
    if (start_boss_hp_bar_anime < FPS / 3 * 2) {
        start_boss_hp_bar_anime++;
        al_draw_bitmap_region(boss_hpfull_bar_img, 0, 0, boss_hpfull_bar_width * (double)start_boss_hp_bar_anime / (FPS / 3 * 2), boss_hpfull_bar_height, window_width - boss_hp_bar_width - 21, 21, 0);
    } else
        al_draw_bitmap_region(boss_hpfull_bar_img, 0, 0, boss_hpfull_bar_width * (double)(boss->get_hp()) / (double)(boss->get_full_hp()), boss_hpfull_bar_height, window_width - boss_hp_bar_width - 21, 21, 0);
}
Interface::~Interface() {
    al_destroy_bitmap(hp_img);
    al_destroy_bitmap(coin_bar_img);
    al_destroy_bitmap(boss_hp_bar_img);
    al_destroy_bitmap(boss_hpfull_bar_img);
    al_destroy_font(interfaceFont);
}
