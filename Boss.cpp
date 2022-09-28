#include "Boss.h"

#include <cmath>
#include <string>

#include "Collide.h"
#include "Config.h"
#define arc(a) (a * 3.14159265 / 180.0)
Boss::Boss(int level) {
    lv = level;
    full_hp = lv * 3000 + 2000;
    hp = full_hp;
    atk = 10 * lv + 20;
    speed = 7;
    money = lv * 500 + 500;
    dir = 0;
    dx = 0, dy = 0;
    state = 1;
    pass_game = 0;

    move.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/boss/enemy";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        move.push_back(new_move_img);
    }
    death.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/boss/enemy_die";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        death.push_back(new_move_img);
    }
    bullet_img.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/bullet/bullet_enemy";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        bullet_img.push_back(new_move_img);
    }
    weakness.reserve(2);
    for (int i = 1; i <= 2; i++) {
        std::string s = "./image/boss/aim";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        weakness.push_back(new_move_img);
    }
    blind_img = al_load_bitmap("./image/effect/blind.png");
    blind_height = al_get_bitmap_height(blind_img);
    blind_width = al_get_bitmap_width(blind_img);
    bul.reserve(200);
    width = al_get_bitmap_width(move[0]);
    height = al_get_bitmap_height(move[0]);
    x = window_width - width - 20;
    y = (window_height - height) / 2;
    is_shooting = 0;
    is_blind = 0;
    is_weak = 0;

    weakness_width = al_get_bitmap_width(weakness[0]);
    weakness_height = al_get_bitmap_height(weakness[0]);

    change_pos_time = FPS / 2;
    move_time = 0;
    move_anime_time = 0;
    move_per_anime_time = FPS;
    weakness_cooldown = 3 * FPS;
    weakness_time = 0;
    blind_time = 0;
    death_anime_time = 0;
    death_anime_end_time = FPS;

    bullet_time = 0;
    bullet_time_end = FPS * 3;
    bullet_degree = 0;
}

void Boss::update() {
    x += dx;
    y += dy;
    if (move_time == 0 && state == 2) {
        dx = 0, dy = 0;
        state = 1;
    }
    if (state != 3) {
        if (change_pos_time == 0) {
            change_pos_time = 8 * FPS + rand() % (int)(4 * FPS);
            state = 2;
            destination_x = 50 + rand() % (window_width - 100 - width);
            destination_y = 50 + rand() % (window_height - 100 - height);
            double distance_x = destination_x - x;
            double distance_y = destination_y - y;
            move_time = sqrt(distance_x * distance_x + distance_y * distance_y) / (double)speed;
            dx = (double)speed * distance_x / sqrt(distance_x * distance_x + distance_y * distance_y);
            dy = (double)speed * distance_y / sqrt(distance_x * distance_x + distance_y * distance_y);
            dir = (distance_x < 0) ? 0 : 1;
        }

        if (is_weak) {
            weakness_time--;
            weakness_x = x + width / 2 + weakness_distance_x - (weakness_width / 2);
            weakness_y = y + height / 2 + weakness_distance_y - (weakness_height / 2);
            if (weakness_time == 0) {
                is_weak = 0;
                weakness_cooldown = 5 * FPS;
            }
        } else {
            weakness_cooldown--;
            if (weakness_cooldown == 0) {
                is_weak = 1;
                int degree = rand() % 360;
                weakness_distance_x = 65 * cos(arc(degree));
                weakness_distance_y = 65 * sin(arc(degree));
                weakness_time = 5 * FPS;
            }
        }
        if (lv == 3) {
            if (is_blind) {
                if (blind_time == FPS * 8) {
                    is_blind = 0;
                    blind_time = 0;
                }
            } else {
                if (blind_time == FPS * 15) {
                    is_blind = 1;
                    blind_time = 0;
                }
            }
            blind_time++;
        }
        if (is_shooting) {
            if (bullet_time % 5 == 0) {
                if (lv == 1) {
                    for (int i = 0; i < 2; i++) {
                        BossBullet *new_bul = new BossBullet(x + width / 2, y + height / 2, atk, bullet_degree + i * 180, &bullet_img);
                        bul.push_back(new_bul);
                    }
                } else {
                    for (int i = 0; i < 4; i++) {
                        BossBullet *new_bul = new BossBullet(x + width / 2, y + height / 2, atk, bullet_degree + i * 90, &bullet_img);
                        bul.push_back(new_bul);
                    }
                }
                if (bullet_pattern == 0) {
                    bullet_degree += 19;
                    if (bullet_degree >= 360)
                        bullet_degree -= 360;
                } else {
                    bullet_degree -= 19;
                    if (bullet_degree <= 0)
                        bullet_degree += 360;
                }
            }
            if (bullet_time == bullet_time_end) {
                bullet_time = 0;
                is_shooting = 0;
                bullet_time_end = 3 * FPS;
            }
        } else {
            if (bullet_time == bullet_time_end) {
                bullet_time = 0;
                is_shooting = 1;
                bullet_time_end = 10 * FPS;
                bullet_degree = rand() % 360;
                bullet_pattern = rand() % 2;
            }
        }
        bullet_time++;
    }
    for (auto iter : bul)
        iter->update();

    for (int i = 0; i < bul.size(); i++) {
        if (bul[i]->get_state() == 3) {
            delete bul[i];
            bul.erase(bul.begin() + i);
            i--;
        }
    }

    if (state == 1)
        change_pos_time--;
    else if (state == 2)
        move_time--;

    if (state == 3) {
        death_anime_time++;
        if (death_anime_time == death_anime_end_time)
            pass_game = 1;
    } else {
        move_anime_time++;
        if (hp < 0) {
            state = 3;
            is_blind = 0;
            dx = 0, dy = 0;
        }
        if (move_anime_time == move_per_anime_time)
            move_anime_time = 0;
    }
}
void Boss::draw() {
    for (auto iter : bul)
        iter->draw();
    if (state == 3) {
        if (death_anime_time < death_anime_end_time / 4)
            al_draw_bitmap(death[0], x, y, dir);
        else if (death_anime_time < death_anime_end_time * 2 / 4)
            al_draw_bitmap(death[1], x, y, dir);
        else if (death_anime_time < death_anime_end_time * 3 / 4)
            al_draw_bitmap(death[2], x, y, dir);
        else
            al_draw_bitmap(death[3], x, y, dir);
    } else {
        if (move_anime_time < move_per_anime_time / 4)
            al_draw_bitmap(move[0], x, y, dir);
        else if (move_anime_time < move_per_anime_time * 2 / 4)
            al_draw_bitmap(move[1], x, y, dir);
        else if (move_anime_time < move_per_anime_time * 3 / 4)
            al_draw_bitmap(move[2], x, y, dir);
        else
            al_draw_bitmap(move[3], x, y, dir);
        // al_draw_filled_circle(x + width / 2, y + height / 2, 65, al_map_rgba(155, 155, 155, 150));
        if (is_weak) {
            if (weakness_time % 30 < 15)
                al_draw_bitmap(weakness[0], weakness_x, weakness_y, 0);
            else
                al_draw_bitmap(weakness[1], weakness_x, weakness_y, 0);
        }
    }
}
void Boss::draw_blind() {
    if (is_blind) {
        al_draw_bitmap(blind_img, blind_x, blind_y, 0);
        for (auto iter : bul)
            iter->blind_draw();
        for (auto iter : *itm)
            iter->blind_draw();
    }
}
Boss::~Boss() {
    for (auto iter : move)
        al_destroy_bitmap(iter);
    for (auto iter : death)
        al_destroy_bitmap(iter);
    for (auto iter : bullet_img)
        al_destroy_bitmap(iter);
    for (auto iter : weakness)
        al_destroy_bitmap(iter);
    al_destroy_bitmap(blind_img);
    for (auto iter : bul)
        delete iter;
}
Boss::Boss() {
    lv = config_read("Boss", "LV");
    full_hp = config_read("Boss", "FULL_HP");
    hp = config_read("Boss", "HP");
    atk = config_read("Boss", "ATK");
    speed = 7;
    money = config_read("Boss", "MONEY");
    dir = config_read("Boss", "DIRECTION");
    dx = config_read_double("Boss", "DX");
    dy = config_read_double("Boss", "DY");
    state = config_read("Boss", "STATE");
    pass_game = config_read("Boss", "PASS_GAME");

    move.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/boss/enemy";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        move.push_back(new_move_img);
    }
    death.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/boss/enemy_die";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        death.push_back(new_move_img);
    }
    bullet_img.reserve(4);
    for (int i = 1; i <= 4; i++) {
        std::string s = "./image/bullet/bullet_enemy";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        bullet_img.push_back(new_move_img);
    }
    weakness.reserve(2);
    for (int i = 1; i <= 2; i++) {
        std::string s = "./image/boss/aim";
        s.insert(s.end(), (char)(i + '0'));
        s.insert(s.length(), ".png");
        ALLEGRO_BITMAP *new_move_img = al_load_bitmap(s.c_str());
        weakness.push_back(new_move_img);
    }
    blind_img = al_load_bitmap("./image/effect/blind.png");
    blind_height = al_get_bitmap_height(blind_img);
    blind_width = al_get_bitmap_width(blind_img);
    bul.reserve(200);
    width = al_get_bitmap_width(move[0]);
    height = al_get_bitmap_height(move[0]);
    x = config_read_double("Boss", "X");
    y = config_read_double("Boss", "Y");
    is_shooting = config_read("Boss", "IS_SHOOTING");
    is_blind = config_read("Boss", "IS_BLIND");
    is_weak = config_read("Boss", "IS_WEAK");

    weakness_width = al_get_bitmap_width(weakness[0]);
    weakness_height = al_get_bitmap_height(weakness[0]);

    change_pos_time = config_read("Boss", "CHANGE_POS_TIME");
    move_time = config_read("Boss", "MOVE_TIME");
    move_anime_time = config_read("Boss", "MOVE_ANIME_TIME");
    move_per_anime_time = FPS;
    weakness_cooldown = config_read("Boss", "WEAKNESS_COOLDOWN");
    weakness_time = config_read("Boss", "WEAKNESS_TIME");
    blind_time = config_read("Boss", "BLIND_TIME");
    death_anime_time = config_read("Boss", "DEATH_ANIME_TIME");
    death_anime_end_time = FPS;

    bullet_time = config_read("Boss", "BULLET_TIME");
    bullet_time_end = config_read("Boss", "BULLET_TIME_END");
    bullet_degree = config_read("Boss", "BULLET_DEGREE");
    blind_x = config_read("Boss", "BLIND_X");
    blind_y = config_read("Boss", "BLIND_Y");
    atk_cool_down = config_read("Boss", "ATK_COOL_DOWN");
    destination_x = config_read("Boss", "DESTINATION_X");
    destination_y = config_read("Boss", "DESTINATION_Y");
    weakness_x = config_read("Boss", "WEAKNESS_X");
    weakness_y = config_read("Boss", "WEAKNESS_Y");
    weakness_distance_x = config_read_double("Boss", "WEAKNESS_DISTANCE_X");
    weakness_distance_y = config_read_double("Boss", "WEAKNESS_DISTANCE_Y");
    bullet_pattern = config_read("Boss", "BULLET_PATTERN");
    int bul_num = config_read("Boss", "BULLET_NUM");
    for (int i = 0; i < bul_num; i++) {
        BossBullet *new_bul = new BossBullet(i, &bullet_img);
        bul.push_back(new_bul);
    }
}
void Boss::SaveGame() {
    config_write("Boss", "BLIND_X", blind_x);
    config_write("Boss", "BLIND_Y", blind_y);
    config_write("Boss", "X", x);
    config_write("Boss", "Y", y);
    config_write("Boss", "DIRECTION", (int)dir);
    config_write("Boss", "STATE", state);
    config_write("Boss", "LV", lv);
    config_write("Boss", "IS_SHOOTING", (int)is_shooting);
    config_write("Boss", "IS_BLIND", (int)is_blind);
    config_write("Boss", "IS_WEAK", (int)is_weak);
    config_write("Boss", "ATK", atk);
    config_write("Boss", "ATK_COOL_DOWN", atk_cool_down);
    config_write("Boss", "FULL_HP", full_hp);
    config_write("Boss", "HP", hp);
    config_write("Boss", "MONEY", money);
    config_write("Boss", "DX", dx);
    config_write("Boss", "DY", dy);
    config_write("Boss", "DESTINATION_X", destination_x);
    config_write("Boss", "DESTINATION_Y", destination_y);
    config_write("Boss", "WEAKNESS_X", weakness_x);
    config_write("Boss", "WEAKNESS_Y", weakness_y);
    config_write("Boss", "WEAKNESS_DISTANCE_X", weakness_distance_x);
    config_write("Boss", "WEAKNESS_DISTANCE_Y", weakness_distance_y);
    config_write("Boss", "CHANGE_POS_TIME", change_pos_time);
    config_write("Boss", "WEAKNESS_TIME", weakness_time);
    config_write("Boss", "WEAKNESS_COOLDOWN", weakness_cooldown);
    config_write("Boss", "MOVE_TIME", move_time);
    config_write("Boss", "DEATH_ANIME_TIME", death_anime_time);
    config_write("Boss", "MOVE_ANIME_TIME", move_anime_time);
    config_write("Boss", "BULLET_TIME", bullet_time);
    config_write("Boss", "BULLET_TIME_END", bullet_time_end);
    config_write("Boss", "BULLET_DEGREE", bullet_degree);
    config_write("Boss", "BULLET_PATTERN", bullet_pattern);
    config_write("Boss", "BLIND_TIME", blind_time);
    config_write("Boss", "PASS_GAME", (int)pass_game);
    config_write("Boss", "BULLET_NUM", (int)bul.size());
    for (int i = 0; i < bul.size(); i++)
        bul[i]->SaveGame(i);
}