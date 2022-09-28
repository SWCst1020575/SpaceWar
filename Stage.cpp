#include "Stage.h"

#include "Collide.h"
#include "Config.h"
Stage::Stage(int stage_num) {
    // TODO: stage_num = 1 : barriers don't have effect.
    // TODO: stage_num = 2 : barriers have effect.
    // TODO: stage_num = 3 = boss : challeng boss, boss has skills.
    bg = al_load_bitmap("./image/stage/battle_bg.png");
    gameover_bg = al_load_bitmap("./image/menu/menu_bg.png");
    heart_img = al_load_bitmap("./image/items/item2.png");
    tool_img = al_load_bitmap("./image/items/item4.png");
    gameover_font = al_load_font("./font/pirulen.ttf", 72, 0);
    stage_type = stage_num;
    is_pause = 0;
    is_change_music = 0;

    bar.reserve(10);
    itm.reserve(5);
    barrier_spawn_time = 0;
    barrier_per_spawn_time = FPS * 1.8;
    item_spawn_time = 0;
    item_per_spawn_time = FPS * 8;

    song = al_load_sample("./sound/fighting.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(sample_instance, (float)config_read("music_vol") / 100.0);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_play_sample_instance(sample_instance);

    enemy_img.reserve(5);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/barrier/barrier";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_enemy_img = al_load_bitmap(s.c_str());
        enemy_img.push_back(new_enemy_img);
    }

    main_char = new Character(config_read("hp"), config_read("atk"), config_read("coin"), stage_type);
    interface = new Interface();
    main_char->init_barrier(&bar);
    difficulty = config_read("difficulty");
    bar_lv = 1;
    stage_time = 0;
    if (stage_type == 3) {
        boss = new Boss(difficulty);
        main_char->init_boss(boss);
        boss_bul = boss->get_bossbullet_pointer();
        boss->init_itm(&itm);
    }
}
int Stage::process(ALLEGRO_EVENT *event) {
    if (is_pause) {
        is_pause = pause->process(event);
        if (!is_pause) {
            if (pause->get_is_scene_change() == 1) {
                delete pause;
                scene_change = 1;
                config_write("coin", main_char->get_money());
                config_write("invincible_time", main_char->get_shield_time());
                return MENU;
            } else if (pause->get_is_scene_change() == 2) {
                delete pause;
                scene_change = 1;
                return EXIT_GAME;
            }
            delete pause;
        }
        return STAGE;
    }
    if (main_char->get_game_state()) {
        if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                scene_change = 1;
                config_write("Stage", "GAME_CONTINUE", 0);
                config_write("coin", main_char->get_money());
                config_write("invincible_time", main_char->get_shield_time());
                config_write("missle", 0);
                return MENU;
            }
        }
        return STAGE;
    }
    if (stage_type == 3) {
        if (boss->get_game_state()) {
            if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    scene_change = 1;
                    config_write("Stage", "GAME_CONTINUE", 0);
                    config_write("coin", main_char->get_money());
                    config_write("invincible_time", main_char->get_shield_time());
                    config_write("missle", 0);
                    return MENU;
                }
            }
            return STAGE;
        }
    }
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event->keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            is_pause = 1;
            pause = new Pause();
            SaveGame();
            return STAGE;
        }
    }
    main_char->process(event);
    return STAGE;
}
void Stage::update() {
    if (stage_type == 3)
        if (boss->get_game_state()) {
            if (!is_change_music) {
                al_destroy_sample(song);
                al_detach_sample_instance(sample_instance);
                song = al_load_sample("./sound/fighting.wav");
                al_reserve_samples(20);
                sample_instance = al_create_sample_instance(song);
                // Loop the song until the display closes
                al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
                al_set_sample_instance_gain(sample_instance, (float)config_read("music_vol") / 100.0);
                al_restore_default_mixer();
                al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
                // set the volume of instance
                al_play_sample_instance(sample_instance);
                is_change_music = 1;
            }
            return;
        }
    if (main_char->get_game_state()) {
        if (!is_change_music) {
            al_destroy_sample(song);
            al_detach_sample_instance(sample_instance);
            song = al_load_sample("./sound/fighting.wav");
            al_reserve_samples(20);
            sample_instance = al_create_sample_instance(song);
            // Loop the song until the display closes
            al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
            al_set_sample_instance_gain(sample_instance, (float)config_read("music_vol") / 100.0);
            al_restore_default_mixer();
            al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
            // set the volume of instance
            al_play_sample_instance(sample_instance);
            is_change_music = 1;
        }
        return;
    }
    if (is_pause)
        return;
    if (stage_type == 3) {
        boss->update_blind_pos(main_char->get_x() + main_char->get_width() / 2, main_char->get_y() + main_char->get_height() / 2);
        boss->update();
        for (auto iter : *boss_bul) {
            if (collide_check(main_char, iter) && iter->get_state() == 1 && main_char->get_state() == 1) {
                iter->set_state(2);
                if (main_char->get_shield_time() > 0) {
                    main_char->subtract_shield_time();
                    main_char->set_state(4);
                } else {
                    damage_compute(main_char, iter->get_damage());
                    main_char->set_state(2);
                }
                continue;
            }
        }
    } else {
        if ((barrier_spawn_time % barrier_per_spawn_time) == 0) {
            Barrier *new_bar = nullptr;
            if (stage_type == 1) {
                int new_barrier_type = rand() % 2;
                new_bar = new Barrier(difficulty, bar_lv, new_barrier_type, enemy_img[new_barrier_type]);
            }
            if (stage_type == 2) {
                int new_barrier_type = rand() % 4;
                new_bar = new Barrier(difficulty, bar_lv, new_barrier_type, enemy_img[new_barrier_type]);
            }
            bar.push_back(new_bar);
            barrier_spawn_time = 0;
            switch (difficulty) {
                case 1:
                    barrier_per_spawn_time = FPS * 1.8 * 0.3 + rand() % (int)(FPS * 1.8 * 0.7);
                    break;
                case 2:
                    barrier_per_spawn_time = FPS * 1.8 * 0.3 + rand() % (int)(FPS * 1.8 * 0.5);
                    break;
                case 3:
                    barrier_per_spawn_time = FPS * 1.8 * 0.3 + rand() % (int)(FPS * 1.8 * 0.3);
                    break;
            }
        }
        if (stage_time % (int)(FPS * 30) == 0)
            bar_lv++;
    }

    if (item_spawn_time == item_per_spawn_time) {
        Item *new_itm = nullptr;
        int rand_num = rand() % 2;
        if (rand_num == 0)
            new_itm = new Item(0, heart_img);
        else
            new_itm = new Item(1, tool_img);
        itm.push_back(new_itm);
        item_spawn_time = 0;
    }
    for (int i = 0; i < bar.size(); i++) {
        bar[i]->update();
        if (collide_check(main_char, bar[i]) && main_char->get_state() == 1) {
            if (main_char->get_shield_time() > 0) {
                main_char->subtract_shield_time();
                main_char->set_state(4);
            } else {
                damage_compute(main_char, bar[i]->get_atk());
                if (bar[i]->get_type() == 3)
                    bar[i]->set_shade_time_count(0);
                main_char->set_state(2);
            }
        }
        if (split_check(main_char, bar[i], 300)) {
            bar[i]->set_split(1);
            bar[i]->set_split_cool_down(FPS * 0.8);
            Barrier *new_bar = new Barrier(bar[i]);
            bar[i]->set_y_speed(20);
            new_bar->set_y_speed(-20);
            bar.push_back(new_bar);
        }
        if (bar[i]->get_state() == 3) {
            if (bar[i]->get_hp() < 0)
                main_char->set_money(bar[i]->get_money() + main_char->get_money());
            delete bar[i];
            bar.erase(bar.begin() + i);
            i--;
        }
    }
    for (int i = 0; i < itm.size(); i++) {
        itm[i]->update();
        if (itm[i]->get_type() == 0) {
            if (collide_check(main_char, itm[i])) {
                main_char->set_hp(main_char->get_hp() + itm[i]->get_hp());
                itm[i]->set_state(2);
            }
        } else if (itm[i]->get_type() == 1) {
            if (main_char->get_shield_time() < 4) {
                if (collide_check(main_char, itm[i])) {
                    main_char->add_shield_time();
                    itm[i]->set_state(2);
                }
            }
        }
        if (itm[i]->get_state() == 2) {
            delete itm[i];
            itm.erase(itm.begin() + i);
            i--;
        }
    }
    main_char->update();
    barrier_spawn_time++;
    item_spawn_time++;
    stage_time++;
}
void Stage::draw() {
    if (stage_type == 3) {
        if (boss->get_game_state()) {
            al_draw_bitmap(gameover_bg, 0, 0, 0);
            al_draw_text(gameover_font, al_map_rgb(255, 255, 255), window_width / 2, window_height / 2 - 30, ALLEGRO_ALIGN_CENTRE, "game clear");
            return;
        }
    }
    if (main_char->get_game_state()) {
        al_draw_bitmap(gameover_bg, 0, 0, 0);
        al_draw_text(gameover_font, al_map_rgb(255, 255, 255), window_width / 2, window_height / 2 - 30, ALLEGRO_ALIGN_CENTRE, "game over");
        return;
    }
    al_draw_bitmap(bg, 0, 0, 0);
    for (auto iter : bar)
        iter->draw();
    for (auto iter : itm)
        iter->draw();

    if (stage_type == 3)
        boss->draw();
    main_char->draw();
    if (stage_type == 3) {
        boss->draw_blind();
        interface->draw(main_char, boss);
    } else
        interface->draw(main_char);
    if (is_pause)
        pause->draw();
}
Stage::~Stage() {
    al_destroy_bitmap(bg);
    al_convert_bitmap(gameover_bg);
    al_destroy_font(gameover_font);
    al_destroy_bitmap(heart_img);
    al_destroy_sample(song);
    al_detach_sample_instance(sample_instance);
    for (auto iter : bar)
        delete iter;
    for (auto iter : itm)
        delete iter;
    for (int i = 0; i < enemy_img.size(); i++)
        al_destroy_bitmap(enemy_img[i]);
    delete interface;
    delete main_char;
    delete text_color;
    delete boss;
}

Stage::Stage() {
    bg = al_load_bitmap("./image/stage/battle_bg.png");
    gameover_bg = al_load_bitmap("./image/menu/menu_bg.png");
    heart_img = al_load_bitmap("./image/items/item2.png");
    tool_img = al_load_bitmap("./image/items/item4.png");
    gameover_font = al_load_font("./font/pirulen.ttf", 72, 0);
    stage_type = config_read("Stage", "STAGE_TYPE");
    is_pause = 1;
    pause = new Pause();

    song = al_load_sample("./sound/fighting.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(sample_instance, (float)config_read("music_vol") / 100.0);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_play_sample_instance(sample_instance);
    is_change_music = 0;
    bar.reserve(10);
    itm.reserve(5);
    barrier_spawn_time = config_read("Stage", "BARRIER_SPAWN_TIME");
    barrier_per_spawn_time = FPS * 1.8;
    item_spawn_time = config_read("Stage", "ITEM_SPAWN_TIME");
    item_per_spawn_time = FPS * 8;

    enemy_img.reserve(5);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/barrier/barrier";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_enemy_img = al_load_bitmap(s.c_str());
        enemy_img.push_back(new_enemy_img);
    }

    main_char = new Character(stage_type);
    interface = new Interface();
    main_char->init_barrier(&bar);
    difficulty = config_read("Stage", "DIFFICULTY");
    bar_lv = config_read("Stage", "BAR_LV");
    stage_time = config_read("Stage", "STAGE_TIME");
    if (stage_type == 3) {
        boss = new Boss();
        main_char->init_boss(boss);
        boss_bul = boss->get_bossbullet_pointer();
        boss->init_itm(&itm);
    }
    int bar_num = config_read("Stage", "BARRIER_NUM");
    for (int i = 0; i < bar_num; i++) {
        Barrier *new_bar = new Barrier(i, &enemy_img);
        bar.push_back(new_bar);
    }
}

void Stage::SaveGame() {
    config_write("Stage", "STAGE_TYPE", stage_type);
    config_write("Stage", "BARRIER_SPAWN_TIME", barrier_spawn_time);
    config_write("Stage", "ITEM_SPAWN_TIME", item_spawn_time);
    config_write("Stage", "STAGE_TIME", (int)stage_time);
    config_write("Stage", "DIFFICULTY", difficulty);
    config_write("Stage", "BAR_LV", bar_lv);
    config_write("Stage", "BARRIER_NUM", (int)bar.size());
    config_write("Stage", "GAME_CONTINUE", 1);
    main_char->SaveGame();
    if (stage_type == 3)
        boss->SaveGame();
    for (int i = 0; i < bar.size(); i++)
        bar[i]->SaveGame(i);
}