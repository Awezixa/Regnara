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
    float pieceX, pieceY; // world position
    int col, row;         // grid position (NEW)

    bool active;
    pieceType type;
    //int count, max;
    //properties
} Piece;

typedef struct Town
{
  int row, col; // position
  bool beingCaptured;//track if king on tile
  int owner;// who owns 0 = neutral, 1 = P1, 2 = P2
  int captureTurns;// check how long king been on tile  
} Town;




void spawnPiece(AppState *app);
void renderPiece(AppState *app);
bool isTileWalkable(int row, int col);

#endif