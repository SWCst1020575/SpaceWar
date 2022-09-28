#include "GameWindow.h"

#include <iostream>

#include "Config.h"
#include "global.h"
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void GameWindow::game_init() {
    char buffer[50];
    if (config_read("Stage", "GAME_CONTINUE") == 1) {
        scene = STAGE;
        origin_scene = STAGE;
        scene_change = 1;
    } else {
        scene = MENU;
        origin_scene = MENU;
        scene_change = 1;
    }
}

bool GameWindow::mouse_hover(int startx, int starty, int width, int height) {
    if (mouse_x >= startx && mouse_x <= startx + width)
        if (mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

void GameWindow::game_play() {
    int msg;

    srand(time(NULL));

    msg = -1;
    game_reset();
    game_begin();

    while (msg != GAME_EXIT) {
        msg = game_run();
    }

    show_err_msg(msg);
}

void GameWindow::show_err_msg(int msg) {
    if (msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow() {
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    timer = al_create_timer(1.0 / FPS);
    /*monster_pro = al_create_timer(1.0 / FPS);

    if (timer == NULL || monster_pro == NULL)
        show_err_msg(-1);*/

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon();    // initialize the font addon
    al_init_ttf_addon();     // initialize the ttf (True Type Font) addon
    al_init_image_addon();   // initialize the image addon
    al_init_acodec_addon();  // initialize acodec addon

    al_install_keyboard();  // install keyboard event
    al_install_mouse();     // install mouse event
    al_install_audio();     // install audio event

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    // al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    game_init();
}

void GameWindow::game_begin() {
    /*
    al_play_sample_instance(backgroundSound);*/

    al_start_timer(timer);
    // al_start_timer(monster_pro);
}

int GameWindow::game_run() {
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {
        error = process_event();
    }
    return error;
}

int GameWindow::game_update() {
    unsigned int i, j;
    switch (scene) {
        case STAGE:
            stage->update();
            break;
        default:
            break;
    }
    return GAME_CONTINUE;
}

void GameWindow::game_reset() {
    // reset game and begin
    /*

    // stop sample instance
    /*al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);
*/
    // stop timer
    al_stop_timer(timer);
    // al_stop_timer(monster_pro);
}

void GameWindow::game_destroy() {
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    al_destroy_timer(timer);
    // al_destroy_timer(monster_pro);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    delete this;
}

int GameWindow::process_event() {
    int i;
    int instruction = GAME_CONTINUE;

    // offset for pause window
    int offsetX = field_width / 2 - 200;
    int offsetY = field_height / 2 - 200;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    // delete original scene and change into new scene
    if (scene_change) {
        switch (scene) {
            case STAGE:
                if (origin_scene != STAGE)
                    enter_stage = menu->get_stage_num();
                delete_scene();
                if (config_read("Stage", "GAME_CONTINUE") == 1)
                    stage = new Stage();
                else
                    stage = new Stage(enter_stage);
                origin_scene = STAGE;
                break;
            case MENU:
                delete_scene();
                menu = new Menu();
                origin_scene = MENU;
                break;
            case ABOUT:
                delete_scene();
                about = new About;
                origin_scene = ABOUT;
                break;
            case SETTING:
                delete_scene();
                setting = new Setting;
                origin_scene = SETTING;
                break;
            case SHOP:
                delete_scene();
                shop = new Shop;
                origin_scene = SHOP;
                break;
            case SKIN:
                delete_scene();
                skin = new Skin;
                origin_scene = SKIN;
                break;
            default:
                break;
        }
        scene_change = 0;
    }
    if (event.type == ALLEGRO_EVENT_TIMER) {
        if (event.timer.source == timer) {
            redraw = true;
        }
    } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_P:
                /*TODO: handle pause event here*/
                break;
        }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (scene == SETTING)
            setting->press();
        else if(scene==SHOP)
            shop->press();
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (scene == SETTING)
            setting->release();
        else if(scene==SHOP)
            shop->release();
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if (scene == MENU)
            menu->text_state_check(mouse_x, mouse_y);
        if (scene == SETTING)
            setting->option_state_check(mouse_x, mouse_y);
        if (scene == SHOP)
            shop->button_state_check(mouse_x, mouse_y);
        if (scene == STAGE)
            stage->update_mouse(mouse_x, mouse_y);
        // menu->MouseIn(mouse_x, mouse_y);
    }
    // the process of each scene
    switch (scene) {
        case STAGE:
            scene = stage->process(&event);
            break;
        case MENU:
            scene = menu->process(&event);
            break;
        case ABOUT:
            scene = about->process(&event);
            break;
        case SETTING:
            scene = setting->process(&event);
            break;
        case SHOP:
            scene = shop->process(&event);
            break;
        case SKIN:
            scene = skin->process(&event);
            break;
        case EXIT_GAME:
            printf("Game exit!\n");
            return GAME_EXIT;
        default:
            break;
    }

    if (redraw && !scene_change) {
        // update each object in game
        instruction = game_update();

        draw();
        redraw = false;
    }

    return instruction;
}

void GameWindow::draw() {
    unsigned int i, j;

    switch (scene) {
        case STAGE:
            stage->draw();
            break;
        case MENU:
            menu->draw();
            break;
        case ABOUT:
            about->draw();
            break;
        case SETTING:
            setting->draw();
            break;
        case SHOP:
            shop->draw();
            break;
        case SKIN:
            skin->draw();
            break;
        default:
            break;
    }

    al_flip_display();
}
void GameWindow::delete_scene() {
    switch (origin_scene) {
        case STAGE:
            delete stage;
            break;
        case MENU:
            delete menu;
            break;
        case ABOUT:
            delete about;
            break;
        case SETTING:
            delete setting;
            break;
        case SHOP:
            delete shop;
            break;
        case SKIN:
            delete skin;
            break;
        default:
            break;
    }
}