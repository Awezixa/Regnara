#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>


// ask cesar about this
typedef enum GameState{
    STATE_MENU,
    STATE_PLAYING,
    TECH_TREE,// can remove (ask cesar)
    STATE_PAUSED,
    STATE_OPTIONS,
    STATE_CONFIRM_MENU,
    STATE_END
} GameState;

//other functions if needed

#endif