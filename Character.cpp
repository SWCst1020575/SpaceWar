#include "Character.h"

#include <string>
#include <utility>

#include "Collide.h"
#include "Config.h"
Character::Character(int char_hp, int char_atk, int char_money, int stage) {
    // load
    skin = (config_read("skin") - 1);
    if (skin == 0) {
        move.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha_move";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
            move.push_back(new_move_img);
        }
        death.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha_death";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_death_img = al_load_bitmap(s.c_str());
            death.push_back(new_death_img);
        }
    } else {
        move.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha1_move";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
            move.push_back(new_move_img);
        }
        death.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha1_death";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_death_img = al_load_bitmap(s.c_str());
            death.push_back(new_death_img);
        }
    }

    bullet_img.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/bullet/bullet";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        bullet_img.push_back(new_move_img);
    }
    missile_img.reserve(6);
    for (int i = 1; i <= 6; i++) {
        std::string s = "./image/bullet/missile";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        missile_img.push_back(new_move_img);
    }
    shield_img.reserve(4);
    for (int i = 1; i <= 6; i++) {
        std::string s = "./image/character/shield";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        shield_img.push_back(new_move_img);
    }
    shield_width = al_get_bitmap_width(shield_img[0]);
    shield_height = al_get_bitmap_height(shield_img[0]);
    bul.reserve(30);
    mis.reserve(3);
    height = al_get_bitmap_height(move[0]);
    width = al_get_bitmap_width(move[0]);
    x = 0;
    y = window_height / 2 - height / 2;
    stage_type = stage;
    dir = 0;
    hp = char_hp;
    limit_hp = hp;
    atk = char_atk;
    money = char_money;
    speed = 6;
    move_anime_time = 0;
    move_per_anime_time = FPS;
    death_anime_time = 0;
    death_anime_end_time = FPS / 3 * 2;
    atk_speed = 10;
    atk_cool_down = 0;
    keyboard.insert(std::make_pair(ALLEGRO_KEY_W, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_A, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_S, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_D, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_SPACE, false));  // shooting

    hurt_no_damage_time = 0;
    create_new_missile_time = 0;
    state = 1;
    gameover = 0;
    auto_shooting = config_read("automation");
    equip = config_read("missle");
    atk_times = 0;
    is_launch_missile = 0;
    shield_time = config_read("invincible_time");
}
void Character::process(ALLEGRO_EVENT *event) {
    // TODO: Control character with keyboard.
    if (state == 3)
        return;
    if (event->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event->keyboard.keycode) {
            case ALLEGRO_KEY_W:
                keyboard[ALLEGRO_KEY_W] = true;
                break;
            case ALLEGRO_KEY_A:
                keyboard[ALLEGRO_KEY_A] = true;
                break;
            case ALLEGRO_KEY_S:
                keyboard[ALLEGRO_KEY_S] = true;
                break;
            case ALLEGRO_KEY_D:
                keyboard[ALLEGRO_KEY_D] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keyboard[ALLEGRO_KEY_SPACE] = true;
                break;
            default:
                break;
        }
    } else if (event->type == ALLEGRO_EVENT_KEY_UP) {
        switch (event->keyboard.keycode) {
            case ALLEGRO_KEY_W:
                keyboard[ALLEGRO_KEY_W] = false;
                break;
            case ALLEGRO_KEY_A:
                keyboard[ALLEGRO_KEY_A] = false;
                break;
            case ALLEGRO_KEY_S:
                keyboard[ALLEGRO_KEY_S] = false;
                break;
            case ALLEGRO_KEY_D:
                keyboard[ALLEGRO_KEY_D] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keyboard[ALLEGRO_KEY_SPACE] = false;
                break;
            default:
                break;
        }
    }
    if (keyboard[ALLEGRO_KEY_W]) {
        if (keyboard[ALLEGRO_KEY_A] || keyboard[ALLEGRO_KEY_D]) {
            dy = -0.707 * speed;
            if (keyboard[ALLEGRO_KEY_A]) {
                dx = -0.707 * speed;
                dir = 1;
            } else {
                dx = 0.707 * speed;
                dir = 0;
            }
        } else {
            dy = -speed;
            dx = 0;
        }
    } else if (keyboard[ALLEGRO_KEY_S]) {
        if (keyboard[ALLEGRO_KEY_A] || keyboard[ALLEGRO_KEY_D]) {
            dy = 0.707 * speed;
            if (keyboard[ALLEGRO_KEY_A]) {
                dx = -0.707 * speed;
                dir = 1;
            } else {
                dx = 0.707 * speed;
                dir = 0;
            }
        } else {
            dy = speed;
            dx = 0;
        }
    } else if (keyboard[ALLEGRO_KEY_A]) {
        dir = 1;
        dx = -speed;
        dy = 0;
    } else if (keyboard[ALLEGRO_KEY_D]) {
        dir = 0;
        dx = speed;
        dy = 0;
    } else {
        dx = 0;
        dy = 0;
    }
}
void Character::update() {
    // TODO: If press space, fire bullets to mouse direction.
    if (state == 3) {
        death_anime_time++;
        if (death_anime_time >= death_anime_end_time * 2)
            gameover = 1;
    }
    x += dx;
    if (x < 0)
        x = 0;
    else if (x + width > window_width)
        x = window_width - width;
    y += dy;
    if (y < 0)
        y = 0;
    else if (y + height > window_height)
        y = window_height - height;

    if (keyboard[ALLEGRO_KEY_SPACE] || auto_shooting) {
        if (atk_cool_down == 0) {
            Bullet *new_bul = new Bullet(x + width / 2, y + height / 2, mouse_x, mouse_y, atk, 15, &bullet_img);
            bul.push_back(new_bul);
        }
        atk_cool_down++;
        if (atk_cool_down == atk_speed)
            atk_cool_down = 0;
    }
    for (int i = 0; i < bul.size(); i++) {
        if (bul[i]->get_state() == 3) {
            delete bul[i];
            bul.erase(bul.begin() + i);
            i--;
        }
    }
    for (auto iter : bul) {
        iter->update();
        if (stage_type == 3) {
            if (collide_check(iter, boss->get_weakness_x() + boss->get_weakness_width() / 2, boss->get_weakness_y() + boss->get_weakness_height() / 2, 22) && iter->get_state() == 1 && boss->get_is_weak()) {
                damage_compute(boss, iter->get_damage());
                iter->set_state(2);
                atk_times++;
                continue;
            }
            if (collide_check(boss, iter, 68) && iter->get_state() == 1) {
                iter->set_state(2);
                atk_times++;
                continue;
            }
        }
        for (int i = 0; i < bar->size(); i++) {
            if (iter->get_state() != 1 || bar->at(i)->get_state() != 1)
                continue;
            if (collide_check(iter, bar->at(i))) {
                damage_compute(bar->at(i), iter->get_damage());
                iter->set_state(2);
                atk_times++;
                break;
            }
        }
    }
    if (equip) {
        for (int i = 0; i < mis.size(); i++) {
            if (mis[i]->get_state() == 3) {
                delete mis[i];
                mis.erase(mis.begin() + i);
                i--;
            }
        }
        if (is_launch_missile) {
            if (stage_type == 3) {
                for (auto iter : mis) {
                    iter->update_target_pos(boss);
                    if (collide_check(iter, boss, 65) && iter->get_state() == 1) {
                        iter->set_state(2);
                        damage_compute(boss, iter->get_damage());
                        continue;
                    }
                    for (auto it : *bar) {
                        if (iter->get_state() != 1 || it->get_state() != 1)
                            continue;
                        if (collide_check(iter, it)) {
                            damage_compute(it, iter->get_damage() * 3);
                            iter->set_state(2);
                            break;
                        }
                    }
                }
            } else {
                for (auto iter : mis) {
                    if (iter->get_state() == 0)
                        continue;
                    iter->update_target_pos((*bar)[missile_target_bar]);
                    for (auto it : *bar) {
                        if (iter->get_state() != 1 || it->get_state() != 1)
                            continue;
                        if (collide_check(iter, it)) {
                            damage_compute(it, iter->get_damage() * 3);
                            iter->set_state(2);
                            break;
                        }
                    }
                }
            }
            if (mis.size() == 0) {
                is_launch_missile = 0;
                atk_times = 0;
            }
        } else {
            if (mis.size() < 3 && state != 3) {
                bool is_create_missile = 0;
                if (create_new_missile_time == 28)
                    create_new_missile_time = 0;
                if (create_new_missile_time == 0) {
                    Missile *new_mis = new Missile(&missile_img, atk);
                    mis.push_back(new_mis);
                }
                create_new_missile_time++;
                if (is_create_missile)
                    create_new_missile_time = 0;
            }
            if (atk_times == 20) {
                is_launch_missile = 1;
                int i = 0;
                if (stage_type != 3)
                    missile_target_bar = rand() % bar->size();
                for (auto iter : mis) {
                    i++;
                    iter->set_state(1);
                    iter->set_path(i);
                    iter->init_speed();
                }
            }
        }
        for (auto iter : mis) {
            iter->update(x, y, width, height, dir);
            if (is_launch_missile) {
                if (stage_type == 3)
                    iter->update_target_pos(boss);
                else {
                    if (missile_target_bar < bar->size())
                        iter->update_target_pos(bar->at(missile_target_bar));
                }
            }
        }
    }
    if (state == 2) {
        hurt_no_damage_time++;
        if (hurt_no_damage_time == FPS) {
            hurt_no_damage_time = 0;
            state = 1;
        }
    } else if (state == 4) {
        hurt_no_damage_time++;
        if (hurt_no_damage_time == FPS) {
            hurt_no_damage_time = 0;
            state = 1;
        }
    }
    if (hp <= 0 && state != 3) {
        state = 3;
        dx = 0;
        dy = 0;
        auto_shooting = 0;
        if (equip)
            for (auto iter : mis)
                iter->set_state(2);
        for (auto iter : keyboard)
            iter.second = false;
    } else if (hp > limit_hp)
        hp = limit_hp;
    if (stage_type == 3)
        if (boss->get_state() != 3 && boss->get_hp() <= 0)
            money += boss->get_money();
    move_anime_time %= move_per_anime_time;
    move_anime_time++;
}
void Character::draw() {
    // al_draw_bitmap(, 0, 0, 0);
    if (equip) {
        for (auto iter : mis)
            iter->draw(1);
    }
    for (auto iter : bul)
        iter->draw();
    if (state == 1 || state == 4) {
        if (move_anime_time < move_per_anime_time / 4)
            al_draw_bitmap(move[0], x, y, dir);
        else if (move_anime_time < move_per_anime_time * 2 / 4)
            al_draw_bitmap(move[1], x, y, dir);
        else if (move_anime_time < move_per_anime_time * 3 / 4)
            al_draw_bitmap(move[2], x, y, dir);
        else
            al_draw_bitmap(move[3], x, y, dir);
    } else if (state == 2) {
        int red_color;
        if (hurt_no_damage_time < (FPS / 2))
            red_color = (double)(FPS / 4 - abs(hurt_no_damage_time - FPS / 4)) / ((double)FPS / 4) * 255;
        else
            red_color = (double)(FPS / 4 - abs(hurt_no_damage_time - FPS * 3 / 4)) / ((double)FPS / 4) * 255;
        if (move_anime_time < move_per_anime_time / 4)
            al_draw_tinted_bitmap(move[0], al_map_rgb(255, red_color, red_color), x, y, dir);
        else if (move_anime_time < move_per_anime_time * 2 / 4)
            al_draw_tinted_bitmap(move[1], al_map_rgb(255, red_color, red_color), x, y, dir);
        else if (move_anime_time < move_per_anime_time * 3 / 4)
            al_draw_tinted_bitmap(move[2], al_map_rgb(255, red_color, red_color), x, y, dir);
        else
            al_draw_tinted_bitmap(move[3], al_map_rgb(255, red_color, red_color), x, y, dir);
    } else if (state == 3) {
        if (death_anime_time < death_anime_end_time / 4)
            al_draw_bitmap(death[0], x, y, dir);
        else if (death_anime_time < death_anime_end_time * 2 / 4)
            al_draw_bitmap(death[1], x, y, dir);
        else if (death_anime_time < death_anime_end_time * 3 / 4)
            al_draw_bitmap(death[2], x, y, dir);
        else if (death_anime_time < death_anime_end_time)
            al_draw_bitmap(death[3], x, y, dir);
    }
    if (equip) {
        for (auto iter : mis)
            iter->draw(0);
    }
    switch (shield_time) {
        case 4:
            al_draw_bitmap(shield_img[0], x + (width - shield_width) / 2, y + (height - shield_height) / 2, 0);
            break;
        case 3:
            al_draw_bitmap(shield_img[1], x + (width - shield_width) / 2, y + (height - shield_height) / 2, 0);
            break;
        case 2:
            al_draw_bitmap(shield_img[2], x + (width - shield_width) / 2, y + (height - shield_height) / 2, 0);
            break;
        case 1:
            al_draw_bitmap(shield_img[3], x + (width - shield_width) / 2, y + (height - shield_height) / 2, 0);
            break;
    }
}
void Character::update_mouse(int m_x, int m_y) {
    mouse_x = m_x;
    mouse_y = m_y;
}
Character::~Character() {
    for (auto iter = move.begin(); iter != move.end(); iter++)
        al_destroy_bitmap(*iter);
    for (auto iter = death.begin(); iter != death.end(); iter++)
        al_destroy_bitmap(*iter);
    for (auto iter = bullet_img.begin(); iter != bullet_img.end(); iter++)
        al_destroy_bitmap(*iter);
    for (auto iter : missile_img)
        al_destroy_bitmap(iter);
    for (auto iter : shield_img)
        al_destroy_bitmap(iter);
    for (auto iter : bul)
        delete iter;
    for (auto iter : mis)
        delete iter;
}
Character::Character(int stage) {
    // load
    skin = config_read("Character", "SKIN");
    if (skin == 0) {
        move.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha_move";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
            move.push_back(new_move_img);
        }
        death.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha_death";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_death_img = al_load_bitmap(s.c_str());
            death.push_back(new_death_img);
        }
    } else {
        move.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha1_move";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
            move.push_back(new_move_img);
        }
        death.reserve(4);
        for (int i = 1; i <= 4; i++) {
            std::string s = "./image/character/cha1_death";
            s.insert(s.end(), (char)(i + '0'));
            s.insert(s.length(), ".png");
            ALLEGRO_BITMAP *new_death_img = al_load_bitmap(s.c_str());
            death.push_back(new_death_img);
        }
    }
    bullet_img.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/bullet/bullet";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        bullet_img.push_back(new_move_img);
    }
    missile_img.reserve(6);
    for (int i = 1; i <= 6; i++) {
        std::string s = "./image/bullet/missile";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        missile_img.push_back(new_move_img);
    }
    shield_img.reserve(4);
    for (int i = 1; i <= 6; i++) {
        std::string s = "./image/character/shield";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        shield_img.push_back(new_move_img);
    }
    shield_width = al_get_bitmap_width(shield_img[0]);
    shield_height = al_get_bitmap_height(shield_img[0]);
    bul.reserve(30);
    mis.reserve(3);
    height = al_get_bitmap_height(move[0]);
    width = al_get_bitmap_width(move[0]);
    x = config_read("Character", "X");
    y = config_read("Character", "Y");
    dx = 0, dy = 0;
    stage_type = stage;
    dir = config_read("Character", "DIRECTION");
    hp = config_read("Character", "HP");
    limit_hp = config_read("Character", "LIMIT_HP");
    atk = config_read("Character", "ATK");
    money = config_read("Character", "MONEY");
    speed = config_read("Character", "SPEED");
    move_anime_time = config_read("Character", "MOVE_ANIME_TIME");
    move_per_anime_time = FPS;
    death_anime_time = config_read("Character", "death_anime_time");
    death_anime_end_time = FPS / 3 * 2;
    atk_speed = 10;
    atk_cool_down = config_read("Character", "ATK_COOLDOWN");
    keyboard.insert(std::make_pair(ALLEGRO_KEY_W, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_A, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_S, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_D, false));
    keyboard.insert(std::make_pair(ALLEGRO_KEY_SPACE, false));  // shooting

    hurt_no_damage_time = config_read("Character", "NO_DAMAGE_TIME");
    create_new_missile_time = config_read("Character", "CREATE_NEW_MISSILE_TIME");
    state = config_read("Character", "STATE");
    gameover = config_read("Character", "GAMEOVER");
    auto_shooting = config_read("Character", "AUTO");
    equip = config_read("Character", "EQUIP");
    atk_times = config_read("Character", "ATK_TIMES");
    is_launch_missile = config_read("Character", "IS_LAUNCH_MISSILE");
    shield_time = config_read("Character", "SHIELD_TIMES");
    missile_target_bar = config_read("Character", "MISSILE_TARGET");

    int bul_num = config_read("Character", "BULLET_NUM");
    int mis_num = config_read("Character", "MISSILE_NUM");

    for (int i = 0; i < bul_num; i++) {
        Bullet *new_bul = new Bullet(i, &bullet_img);
        bul.push_back(new_bul);
    }
    for (int i = 0; i < mis_num; i++) {
        Missile *new_mis = new Missile(i, &missile_img);
        mis.push_back(new_mis);
    }
}
void Character::SaveGame() {
    config_write("Character", "X", x);
    config_write("Character", "Y", y);
    config_write("Character", "DIRECTION", (int)dir);
    config_write("Character", "EQUIP", (int)equip);
    config_write("Character", "SKIN", (int)skin);
    config_write("Character", "AUTO", (int)auto_shooting);
    config_write("Character", "STATE", state);
    config_write("Character", "ATK_TIMES", atk_times);
    config_write("Character", "IS_LAUNCH_MISSILE", (int)is_launch_missile);
    config_write("Character", "MISSILE_TARGET", missile_target_bar);
    config_write("Character", "ATK", atk);
    config_write("Character", "ATK_COOLDOWN", atk_cool_down);
    config_write("Character", "HP", hp);
    config_write("Character", "LIMIT_HP", limit_hp);
    config_write("Character", "MONEY", money);
    config_write("Character", "SPEED", speed);
    config_write("Character", "SHIELD_TIMES", shield_time);
    config_write("Character", "MOVE_ANIME_TIME", move_anime_time);
    config_write("Character", "NO_DAMAGE_TIME", hurt_no_damage_time);
    config_write("Character", "DEATH_ANIME_TIME", death_anime_time);
    config_write("Character", "CREATE_NEW_MISSILE_TIME", create_new_missile_time);
    config_write("Character", "GAMEOVER", (int)gameover);
    config_write("Character", "BULLET_NUM", (int)bul.size());
    config_write("Character", "MISSILE_NUM", (int)mis.size());
    for (int i = 0; i < bul.size(); i++)
        bul[i]->SaveGame(i);
    for (int i = 0; i < mis.size(); i++)
        mis[i]->SaveGame(i);
}