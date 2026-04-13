#ifndef PLAYER_H
#define PLAYER_H

typedef struct AppState AppState;

typedef struct Player1
{
    /*
        other properties
        * Num towns & pieces
        * turns can be global/ visible to both players
        * other stuff
        * 
    */
    int p1Gold;
    int towns;
}Player1;

typedef struct Player2
{
    int p2Gold;
    int towns;
}Player2;



#endif