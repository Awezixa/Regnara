#ifndef CHEATS_H
#define CHEATS_H

#include "../game.h"

typedef struct AppState AppState;

/* Cheats needed

* piece teleportation
* pieces can spawn anywhere
* infinite money

*/

void infiniteMoney(AppState *app);
void presetMatch(AppState *app);
void drawCheats(AppState *app);


#endif