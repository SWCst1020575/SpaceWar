#include "Setting.h"

Setting::Setting() {
    bg = al_load_bitmap("./image/setting/setting_bg.png");
    settingtext = al_load_ttf_font("./font/pirulen.ttf", 60, 0);

    music_vol = new Setting_option;
    sound_effect_vol = new Setting_option;
    automation = new Setting_option;
    difficulty = new Setting_option;

    music_vol->init("music_vol");
    sound_effect_vol->init("sound_effect_vol");
    automation->init("automation");
    difficulty->init("difficulty");
}
int Setting::process(ALLEGRO_EVENT *event) {
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            scene_change = 1;
            return MENU;
        }
    }
    return SETTING;
}
void Setting::draw() {
    al_draw_bitmap(bg, 0, 0, 0);
    al_draw_text(settingtext, al_map_rgb(181, 223, 235), 180, 30, ALLEGRO_ALIGN_CENTRE, "SETTING");
    music_vol->draw(mouse_x);
    sound_effect_vol->draw(mouse_x);
    automation->draw(mouse_x);
    difficulty->draw(mouse_x);
}
void Setting::option_state_check(int x, int y){
    mouse_x = x;
    mouse_y = y;
    music_vol->hover(mouse_x, mouse_y);
    sound_effect_vol->hover(mouse_x, mouse_y);
    automation->hover(mouse_x, mouse_y);
    difficulty->hover(mouse_x, mouse_y);
}
void Setting::press(){
    music_vol->press(mouse_x);
    sound_effect_vol->press(mouse_x);
    automation->press(mouse_x);
    difficulty->press(mouse_x);
}
void Setting::release(){
    music_vol->release();
    sound_effect_vol->release();
    automation->release();
    difficulty->release();
}
Setting::~Setting() {
    al_destroy_bitmap(bg);
    al_destroy_font(settingtext);
    delete text_color;
}


void Setting_option::init(const char* option_char){
    option.assign(option_char, strlen(option_char));
    val = config_read(option.c_str());
    numtext = al_load_ttf_font("./font/pirulen.ttf", 40, 0);
    move_anime_time = 0;
    move_per_anime_time = FPS;

    if(option=="music_vol" || option=="sound_effect_vol" || option=="difficulty"){
        shape_white = al_load_bitmap("./image/setting/cir_white.png");
        shape_blue = al_load_bitmap("./image/setting/cir_blue.png");
        region_width =  al_get_bitmap_width(shape_white);
        region_height = al_get_bitmap_height(shape_white);
        if(option=="difficulty"){
            x = 950 + (val*389/100);
            y = 410;
        }
        else{
            x = 950+ (val*389/100);
            y = (option=="music_vol") ? 135 : 277;
        }
    }
    else{
        shape_white = al_load_bitmap("./image/setting/rec_white.png");
        shape_blue = al_load_bitmap("./image/setting/rec_blue.png");
        region_width =  al_get_bitmap_width(shape_white);
        region_height = al_get_bitmap_height(shape_white);
        x = 950;
        y = 558;
    }


}

void Setting_option::press(int mousex){click=true;dis = mousex-x;}
void Setting_option::draw(int mouse_x){
    move_anime_time %= move_per_anime_time;
    if(move_anime_time==0 || move_anime_time==FPS/2) flag=false;
    move_anime_time++;
    if(option=="music_vol" || option=="sound_effect_vol"){
            al_draw_text(numtext, al_map_rgb(181, 223, 235), 1450, y+8, ALLEGRO_ALIGN_CENTRE, (std::to_string(val)).c_str());
            if(click&&state){
                if(x>=944 && x<=1335){
                    x = mouse_x - dis;
                    if((x-945)*100/389>=100) val = 100;
                    else if((x-945)*100/389<=0) val = 0;
                    else val = (x-945)*100/389;
                    al_draw_bitmap(shape_blue, x, y, 0);
                }
                else al_draw_bitmap(shape_blue, x, y, 0);
            }
            else{
                x = 945+(val*389/100);
                al_draw_bitmap(shape_white, x, y, 0);
                config_write(option.c_str(), val);
            }
    }
    else if(option=="difficulty"){
        al_draw_text(numtext, al_map_rgb(181, 223, 235), 1450, y+8, ALLEGRO_ALIGN_CENTRE, (std::to_string(val)).c_str());
            if(click&&state){
                if(mouse_x>=945 && mouse_x<=1339){
                    x = mouse_x - dis;
                    val = (x-945)*3/389 + 1 ;
                    al_draw_bitmap(shape_blue, x, y, 0);
                }
            }
            else{
                x = 945 + (380*(val-1))/2 ;
                al_draw_bitmap(shape_white, x, y, 0);
                config_write(option.c_str(), val);
            }
    }
    else{
        if(click&&state&&!flag){
            val = (!val);
            config_write("automation", val);
            flag = true;
        }
        if(val) al_draw_bitmap(shape_blue, x+80, y, 0);
        else al_draw_bitmap(shape_white, x+20, y, 0);
        al_draw_text(numtext, al_map_rgb(181, 223, 235), 1450, y+8, ALLEGRO_ALIGN_CENTRE, (val)?"ON":"OFF");
    }

}

void Setting_option::hover(int mouse_x, int mouse_y){
    if (mouse_x >= x && mouse_x <= (x + region_width) && mouse_y >= y && mouse_y <= (y + region_height))
        state = 1;
    else
        state = 0;
}
