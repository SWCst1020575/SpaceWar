#include "Item.h"

#include "Collide.h"
Item::Item(int item_type, ALLEGRO_BITMAP* item_img) {
    img = item_img;
    width = al_get_bitmap_width(img);
    height = al_get_bitmap_height(img);
    x = window_width + window_width;
    y = rand() % (window_height - height);
    type = item_type;  // 0=heart
    speed = 5;
    dx = -speed;
    dy = 0;
    state = 1;
    if (type == 0) {
        hp = 100;
        money = 0;
    }
    blind_time = 0;
}

void Item::update() {
    x += dx;
    y += dy;
    
    if (blind_time = 0)
        blind_dt = 1;
    else if (blind_time == FPS)
        blind_dt = -1;
    blind_time += blind_dt;
}
void Item::draw() {
    al_draw_bitmap(img, x, y, 0);
}
void Item::blind_draw() {
    int color = (double)blind_time / 60.0;
    al_draw_tinted_bitmap(img, al_map_rgba(color, color, color, color), x, y, 0);
}