#ifndef STAGE_H_INCLUDED
#define STAGE_H_INCLUDED
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>

#include <string>
#include <vector>

#include "Barrier.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Character.h"
#include "Interface.h"
#include "Item.h"
#include "Pause.h"
#include "global.h"
class Stage {
   public:
    // initial game with new game
    Stage(int);
    // initial game with save
    Stage();
    int process(ALLEGRO_EVENT *);
    void update_mouse(int mouse_x, int mouse_y) {
        main_char->update_mouse(mouse_x, mouse_y);
        if (is_pause)
            pause->text_state_check(mouse_x, mouse_y);
    }
    void update();
    void draw();
    void SaveGame();
    ~Stage();

   private:
    ALLEGRO_SAMPLE *song = NULL;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_BITMAP *bg = NULL;
    ALLEGRO_BITMAP *gameover_bg = NULL;
    ALLEGRO_FONT *gameover_font = NULL;
    ALLEGRO_COLOR *text_color = NULL;
    Interface *interface = NULL;
    std::vector<ALLEGRO_BITMAP *> enemy_img;
    ALLEGRO_BITMAP *heart_img = NULL;
    ALLEGRO_BITMAP *tool_img = NULL;
    int stage_type;
    bool is_pause;
    bool is_change_music;

    Character *main_char = NULL;
    std::vector<Barrier *> bar;
    std::vector<BossBullet *> *boss_bul;
    std::vector<Item *> itm;
    Boss *boss = nullptr;
    Pause *pause = nullptr;

    int barrier_spawn_time;
    int barrier_per_spawn_time;

    int item_spawn_time;
    int item_per_spawn_time;

    long long int stage_time;
    int difficulty;
    int bar_lv;
};
#endif