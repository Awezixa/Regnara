#ifndef PIECE_H
#define PIECE_H
#define MAX_PIECES 16
#include <stdbool.h>
typedef struct AppState AppState;

/* Needed functions
- piece spawning
- piece movement
- capturing  
- upgrade

Other needed things
- figure out how to swap piece textures
*/
typedef enum pieceType{
    KING,
    QUEEN,
    PAWN,
    ROOK,
    BISHOP,
    KNIGHT,
    /*
    * MAGE
    * FLANKER
    * CATAPAULT
    */

} pieceType;
typedef struct Piece
{
    float pieceX, pieceY;
    bool active;
    pieceType type;
    //int count, max;
    //properties
} Piece;

void spawnPiece(AppState *app);
void renderPiece(AppState *app);


#endif