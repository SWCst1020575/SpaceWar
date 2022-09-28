#include "Shop.h"

Shop::Shop() {
    bg = al_load_bitmap("./image/shop/shop_bg.png");
    coin = al_load_bitmap("./image/shop/coin.png");
    shop_font = al_load_font("./font/pirulen.ttf",60,0);
    money = al_load_font("./font/pirulen.ttf",38,0);

    skin1 = new Chara_skin;
    skin2 = new Chara_skin;
    missle = new Missle;
    shield = new Container;
    atk = new Container;
    hp = new Container;

    skin1->init("skin1");
    skin2->init("skin2");
    missle->init("missle");
    shield->init("invincible_time");
    atk->init("atk");
    hp->init("hp");
}
int Shop::process(ALLEGRO_EVENT *event) {
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            scene_change = 1;
            return MENU;
        }
    }
    return SHOP;
}
void Shop::draw() {
    al_draw_bitmap(bg, 0, 0, 0);
    al_draw_bitmap(coin, 20, 700, 0);
    al_draw_text(shop_font, al_map_rgb(181, 223, 235), 180, 30, ALLEGRO_ALIGN_CENTRE, "SHOP");
    al_draw_text(money, al_map_rgb(181, 223, 235),150, 705, ALLEGRO_ALIGN_CENTRE, (std::to_string(config_read("coin"))).c_str());
    skin1->draw(700, 130);
    skin2->draw(1000, 130);
    missle->draw(700, 320);
    shield->draw(700, 480);
    atk->draw(700, 580);
    hp->draw(700, 680);
}
void Shop::button_state_check(int mouse_x, int mouse_y){
    skin1->hover(mouse_x, mouse_y);
    skin2->hover(mouse_x, mouse_y);
    missle->hover(mouse_x, mouse_y);
    shield->hover(mouse_x, mouse_y);
    atk->hover(mouse_x, mouse_y);
    hp->hover(mouse_x, mouse_y);
}
void Shop::press(){
    skin1->press();
    skin2->press();
    missle->press();
    shield->press();
    atk->press();
    hp->press();
}
void Shop::release(){
    skin1->release();
    skin2->release();
    missle->release();
    shield->release();
    atk->release();
    hp->release();
}
Shop::~Shop() {
    al_destroy_bitmap(bg);
    al_destroy_bitmap(coin);
    al_destroy_font(money);
    al_destroy_font(shop_font);
    delete skin1;
    delete skin2;
    delete missle;
    delete shield;
    delete atk;
    delete hp;
}


void Chara_skin::init(const char* option_char){
    option.assign(option_char, strlen(option_char));
    avail = config_read((option+"_avail").c_str());
    buy_font = al_load_font("./font/pirulen.ttf",38,0);
    for(int i=1; i<=4; i++){
        std::string s = "./image/shop/";
        s = s+option+"_move"+(char)(i+'0')+".png";
        skin[i-1] = al_load_bitmap(s.c_str());
    }
    std::string s = "./image/shop/";
    s= s+option+"_grey.png";
    skin_grey = al_load_bitmap(s.c_str());
    region_width =  al_get_bitmap_width(skin_grey);
    region_height = al_get_bitmap_height(skin_grey);
    move_anime_time = 0;
    move_per_anime_time = FPS;
}
void Chara_skin::draw(int dx, int dy){
    x = dx;
    y = dy;
    move_anime_time %= move_per_anime_time;
    move_anime_time++;
    skin_now = config_read("skin");
    std::string s = "skin";
    s += (char)(skin_now +'0');
    if(s==option){
        if (move_anime_time < move_per_anime_time / 4)
            al_draw_bitmap(skin[0], x, y, 0);
        else if (move_anime_time < move_per_anime_time * 2 / 4)
            al_draw_bitmap(skin[1], x, y, 0);
        else if (move_anime_time < move_per_anime_time * 3 / 4)
            al_draw_bitmap(skin[2], x, y, 0);
        else
            al_draw_bitmap(skin[3], x, y, 0);
    }
    else{
        al_draw_bitmap(skin_grey, x, y, 0);
        if(state==true && avail==0 && config_read("coin")>=SKIN_PRICE){
            al_draw_text(buy_font, al_map_rgb(181, 223, 235), x+55, y, ALLEGRO_ALIGN_CENTRE, "BUY");
            if(click==true){
                avail = 1;
                config_write((option+"_avail").c_str(), 1);
                int coin_buf = config_read("coin") - SKIN_PRICE;
                config_write("coin", coin_buf);
            }
        }
        else if(state&&click&&avail){
            config_write("skin", (skin_now==1)?2:1);
        }
    }
}
void Chara_skin::hover(int mouse_x, int mouse_y){
    if (mouse_x >= x && mouse_x <= (x + region_width) && mouse_y >= y && mouse_y <= (y + region_height))
        state = true;
    else
        state = false;
}
void Missle::init(const char* option_char){
    option.assign(option_char, strlen(option_char));
    avail = config_read("missle");
    buy_font = al_load_font("./font/pirulen.ttf",38,0);
    skin1_grey = al_load_bitmap("./image/shop/skin1_equipped_grey.png");
    skin2_grey = al_load_bitmap("./image/shop/skin2_equipped_grey.png");
    for(int i=1; i<=4; i++){
        std::string s = "./image/shop/skin1_equipped";
        s = s+(char)(i+'0')+".png";
        skin1[i-1] = al_load_bitmap(s.c_str());
        s = "./image/shop/skin2_equipped";
        s = s+(char)(i+'0')+".png";
        skin2[i-1] = al_load_bitmap(s.c_str());
    }
    region_width =  al_get_bitmap_width(skin1_grey);
    region_height = al_get_bitmap_height(skin1_grey);
    move_anime_time = 0;
    move_per_anime_time = FPS;
}
void Missle::draw(int dx, int dy){
    x = dx;
    y = dy;
    move_anime_time %= move_per_anime_time;
    move_anime_time++;
    skin_now = config_read("skin");
    if(avail==1){
        if (move_anime_time < move_per_anime_time / 4)
            (skin_now==1) ? al_draw_bitmap(skin1[0], x, y, 0) : al_draw_bitmap(skin2[0], x, y, 0);
        else if (move_anime_time < move_per_anime_time * 2 / 4)
            (skin_now==1) ? al_draw_bitmap(skin1[1], x, y, 0) : al_draw_bitmap(skin2[1], x, y, 0);
        else if (move_anime_time < move_per_anime_time * 3 / 4)
            (skin_now==1) ? al_draw_bitmap(skin1[2], x, y, 0) : al_draw_bitmap(skin2[2], x, y, 0);
        else
            (skin_now==1) ? al_draw_bitmap(skin1[3], x, y, 0) : al_draw_bitmap(skin2[3], x, y, 0);
    }
    else{
        (skin_now==1) ? al_draw_bitmap(skin1_grey, x, y, 0) : al_draw_bitmap(skin2_grey, x, y, 0);
        if(state==true && config_read("coin")>=SKIN_PRICE){
            al_draw_text(buy_font, al_map_rgb(181, 223, 235), x+55, y+38, ALLEGRO_ALIGN_CENTRE, "BUY");
            if(click==true){
                avail = 1;
                config_write("missle", 1);
                int coin_buf = config_read("coin") - SKIN_PRICE;
                config_write("coin", coin_buf);
            }
        }
    }
}
void Missle::hover(int mouse_x, int mouse_y){
    if (mouse_x >= x && mouse_x <= (x + region_width) && mouse_y >= y && mouse_y <= (y + region_height))
        state = true;
    else
        state = false;
}

void Container::init(const char* option_char){
    option.assign(option_char, strlen(option_char));
    quan = config_read(option.c_str());
    quan_font = al_load_font("./font/pirulen.ttf",38,0);
    std::string s = "./image/shop/";
    s = s + option + ".png";
    item = al_load_bitmap(s.c_str());
    sub = al_load_bitmap("./image/shop/sub.png");
    add = al_load_bitmap("./image/shop/add.png");
    region_width = al_get_bitmap_width(sub);
    region_height = al_get_bitmap_height(sub);
    move_anime_time = 0;
    move_per_anime_time = FPS;
}
void Container::draw(int dx, int dy){
    x = dx;
    y = dy;
    move_anime_time %= move_per_anime_time;
    if(move_anime_time==0 || move_anime_time==FPS/2) flag=false;
    move_anime_time++;
    al_draw_bitmap(item, x, y, 0);
    al_draw_bitmap(sub, x+dis1, y, 0);
    al_draw_bitmap(add, x+dis1+dis2, y, 0);
    al_draw_text(quan_font, al_map_rgb(181, 223, 235), x+dis1+(dis2/2)+15, y, ALLEGRO_ALIGN_CENTRE, (std::to_string(config_read(option.c_str()))).c_str());
    if(!flag && click){
        int val = config_read(option.c_str());
        int coin = config_read("coin");
        if(state_sub && click){
            if(option=="invincible_time" && val-SHIELD_UNIT>=0){
                val -= SHIELD_UNIT;
                coin += SHIELD_PRICE;
                config_write("invincible_time", val);
            }
            if(option=="atk" && val-ATK_UNIT>0){
                val -= ATK_UNIT;
                coin += ATK_PRICE;
                config_write("atk", val);
            }
            if(option=="hp" && val-HP_UNIT>0){
                val -= HP_UNIT;
                coin += HP_PRICE;
                config_write("hp", val);
            }
            config_write("coin", coin);
        }
        if(state_add && click){
            if(option=="invincible_time" && val+SHIELD_UNIT<=SHIELD_MAX && coin-SHIELD_PRICE>=0){
                val += SHIELD_UNIT;
                coin -= SHIELD_PRICE;
                config_write("invincible_time", val);
            }
            if(option=="atk" && val+ATK_UNIT<=ATK_MAX && coin-ATK_PRICE>=0){
                val += ATK_UNIT;
                coin -= ATK_PRICE;
                config_write("atk", val);
            }
            if(option=="hp" && val+HP_UNIT<=HP_MAX && coin-HP_PRICE>=0){
                val += HP_UNIT;
                coin -= HP_PRICE;
                config_write("hp", val);
            }
            config_write("coin", coin);
        }
        flag = true;
    }
}
void Container::hover(int mouse_x, int mouse_y){
    if (mouse_x >= x+dis1 && mouse_x <= (x+dis1 + region_width) && mouse_y >= y && mouse_y <= (y + region_height))
        state_sub = true;
    else
        state_sub = false;
    if (mouse_x >= x+dis1+dis2 && mouse_x <= (x+dis1+dis2 + region_width) && mouse_y >= y && mouse_y <= (y + region_height))
        state_add = true;
    else
        state_add = false;
}
