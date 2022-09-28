#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro5/allegro_font.h>

#define font_size 12
#define grid_width 40
#define grid_height 40

#define window_width 1600
#define window_height 800
#define field_width 600
#define field_height 600

#define Num_TowerType 5
#define Num_MonsterType 4
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)

extern int TowerRadius[];
extern char TowerClass[][20];
extern int TowerWidth[];
extern int TowerHeight[];
extern bool scene_change;
extern int bgm_volume;
extern int se_volume;
const float FPS = 60;
enum { ARCANE = 0,
       ARCHER,
       CANON,
       POISON,
       STORM };
enum { WOLF = 0,
       WOLFKNIGHT,
       DEMONNIJIA,
       CAVEMAN };
enum { MENU = 0,
       ABOUT,
       SETTING,
       SKIN,
       SHOP,
       STAGE,
       EXIT_GAME };

#endif  // GLOBAL_H_INCLUDED
