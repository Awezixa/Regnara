#ifndef PIECE_H
#define PIECE_H
#define MAX_PIECES 8
#include <stdbool.h>
typedef struct AppState AppState;

/* Needed functions
- piece spawning
- piece movement
- capturing  
- upgrade

Other needed things
- piece struct
- piece limit
- figure out how to swap piece textures
*/

typedef struct Piece
{
    float pieceX, pieceY;
    bool active;
    //int count, max;
    //properties
} Piece;

void spawnPiece(AppState *app);
void renderPiece(AppState *app);


#endif