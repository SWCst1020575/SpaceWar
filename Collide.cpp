#include "Collide.h"

bool collide_check(Bullet *bul, Barrier *bar) {
    if (((bul->get_x() >= bar->get_x() && bul->get_x() <= (bar->get_x() + bar->get_width())) ||
         ((bul->get_x() + bul->get_width()) >= bar->get_x() && (bul->get_x() + bul->get_width()) <= (bar->get_x() + bar->get_width()))) &&
        ((bul->get_y() >= bar->get_y() && bul->get_y() <= (bar->get_y() + bar->get_height())) ||
         ((bul->get_y() + bul->get_height()) >= bar->get_y() && (bul->get_y() + bul->get_height()) <= (bar->get_y() + bar->get_height()))))
        return true;
    return false;
}
bool collide_check(Character *cha, Barrier *bar) {
    if (((cha->get_x() >= bar->get_x() && cha->get_x() <= (bar->get_x() + bar->get_width())) ||
         ((cha->get_x() + cha->get_width()) >= bar->get_x() && (cha->get_x() + cha->get_width()) <= (bar->get_x() + bar->get_width()))) &&
        ((cha->get_y() >= bar->get_y() && cha->get_y() <= (bar->get_y() + bar->get_height())) ||
         ((cha->get_y() + cha->get_height()) >= bar->get_y() && (cha->get_y() + cha->get_height()) <= (bar->get_y() + bar->get_height()))))
        return true;
    return false;
}
bool collide_check(Character *cha, Item *itm) {
    if (((cha->get_x() >= itm->get_x() && cha->get_x() <= (itm->get_x() + itm->get_width())) ||
         ((cha->get_x() + cha->get_width()) >= itm->get_x() && (cha->get_x() + cha->get_width()) <= (itm->get_x() + itm->get_width()))) &&
        ((cha->get_y() >= itm->get_y() && cha->get_y() <= (itm->get_y() + itm->get_height())) ||
         ((cha->get_y() + cha->get_height()) >= itm->get_y() && (cha->get_y() + cha->get_height()) <= (itm->get_y() + itm->get_height()))))
        return true;
    return false;
}
bool collide_check(Boss *boss, Bullet *bul, int r) {
    int boss_mid_x = boss->get_x() + boss->get_width() / 2;
    int boss_mid_y = boss->get_y() + boss->get_height() / 2;
    int bul_mid_x = bul->get_x() + bul->get_width() / 2;
    int bul_mid_y = bul->get_y() + bul->get_height() / 2;
    if ((boss_mid_x - bul_mid_x) * (boss_mid_x - bul_mid_x) + (boss_mid_y - bul_mid_y) * (boss_mid_y - bul_mid_y) < r * r)
        return 1;
    return 0;
}
bool collide_check(Bullet *bul, int weakness_x, int weakness_y, int r) {
    int bul_x = bul->get_x() + bul->get_width() / 2;
    int bul_y = bul->get_y() + bul->get_height() / 2;
    if ((bul_x - weakness_x) * (bul_x - weakness_x) + (bul_y - weakness_y) * (bul_y - weakness_y) < r * r)
        return 1;
    return 0;
}
bool collide_check(Missile *mis, Barrier *bar) {
    int mis_mid_x = mis->get_x() + mis->get_width() / 2;
    int mis_mid_y = mis->get_y() + mis->get_height() / 2;
    if (mis_mid_x >= bar->get_x() && mis_mid_x <= bar->get_x() + bar->get_width() &&
        mis_mid_y >= bar->get_y() && mis_mid_y <= bar->get_y() + bar->get_height())
        return true;
    return false;
}
bool collide_check(Missile *mis, Boss *boss, int r) {
    int boss_mid_x = boss->get_x() + boss->get_width() / 2;
    int boss_mid_y = boss->get_y() + boss->get_height() / 2;
    int mis_mid_x = mis->get_x() + mis->get_width() / 2;
    int mis_mid_y = mis->get_y() + mis->get_height() / 2;
    if ((boss_mid_x - mis_mid_x) * (boss_mid_x - mis_mid_x) + (boss_mid_y - mis_mid_y) * (boss_mid_y - mis_mid_y) < r * r)
        return 1;
    return 0;
}
bool collide_check(Character *cha, BossBullet *bul) {
    int char_x;
    int r = 60 * 60;
    if (cha->get_direction())
        char_x = cha->get_x() + cha->get_width() / 2 - 35;
    else
        char_x = cha->get_x() + cha->get_width() / 2 + 35;
    int char_y = cha->get_y() + cha->get_height();
    int bul_x = bul->get_x() + bul->get_width();
    int bul_y = bul->get_y() + bul->get_height();
    if ((char_x - bul_x) * (char_x - bul_x) + (char_y - bul_y) * (char_y - bul_y) < r)
        return true;
    return false;
}
bool split_check(Character *cha, Barrier *bar, int r) {
    if (bar->get_type() != 2 || bar->get_split())
        return 0;
    int cha_x = cha->get_x() + cha->get_width() / 2;
    int cha_y = cha->get_y() + cha->get_height() / 2;
    int bar_x = bar->get_x() + bar->get_width() / 2;
    int bar_y = bar->get_y() + bar->get_height() / 2;
    if ((cha_x - bar_x) * (cha_x - bar_x) + (cha_y - bar_y) * (cha_y - bar_y) < r * r)
        return 1;
    return 0;
}
void damage_compute(Character *cha, int val) { cha->set_hp(cha->get_hp() - val); }
void damage_compute(Barrier *bar, int val) { bar->set_hp(bar->get_hp() - val); }
void damage_compute(Boss *boss, int val) { boss->set_hp(boss->get_hp() - val); }