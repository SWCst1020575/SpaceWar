#ifndef COLLIDE_H_INCLUDED
#define COLLIDE_H_INCLUDED
#include "Barrier.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Bullet.h"
#include "Character.h"
#include "Item.h"
#include "Missile.h"
bool collide_check(Bullet *, Barrier *);
bool collide_check(Character *, Barrier *);
bool collide_check(Character *, Item *);
bool collide_check(Boss *, Bullet *, int);
bool collide_check(Bullet *, int, int, int);  // check weakness midpoint of weakness
bool collide_check(Missile *, Barrier *);
bool collide_check(Missile *, Boss *, int);
bool collide_check(Character *, BossBullet *);
bool split_check(Character *, Barrier *, int);
void damage_compute(Character *, int);
void damage_compute(Barrier *, int);
void damage_compute(Boss *, int);
#endif