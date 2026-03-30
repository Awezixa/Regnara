#ifndef PIECE_H
#define PIECE_H
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
    bool piecePlaced;
    float pieceX, pieceY;
    int count, max;
    //properties
} Piece;

void spawnPiece(AppState *app);
void renderPiece(AppState *app);


#endif