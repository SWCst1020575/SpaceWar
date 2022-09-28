#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <time.h>

#include <list>
#include <vector>

#include "About.h"
#include "Menu.h"
#include "Setting.h"
#include "Shop.h"
#include "Skin.h"
#include "Stage.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7

// clock rate

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

class GameWindow {
   public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();

    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    // each drawing scene function
    void draw();

    // process of updated event
    int process_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);

    void delete_scene();

   public:
    bool initial = true;

   private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *background = NULL;

    ALLEGRO_DISPLAY *display = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_SAMPLE *song = NULL;
    ALLEGRO_SAMPLE_INSTANCE *song_volume = NULL;
    int scene;
    int origin_scene;

    Menu *menu = NULL;
    About *about = NULL;
    Setting *setting = NULL;
    Skin *skin = NULL;
    Stage *stage = NULL;
    Shop *shop = NULL;

    int mouse_x, mouse_y;

    bool redraw = false;
    bool mute = false;
    int enter_stage;
};

#endif  // MAINWINDOW_H_INCLUDED
