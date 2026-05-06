#ifndef PIECE_H
#define PIECE_H
 //should make limit higher for cheats and change from dynamic array to dynamic storage for more pieces and larger maps
#include <stdbool.h>
typedef struct AppState AppState;

/* Needed functions  
- upgrade

Piece costs
- Pawn: 1
- Queen: 6
- Knight: 2
- Bishop: 4
- Rook: 3
*/



typedef enum pieceType{
    KING,
    QUEEN,
    PAWN,
    ROOK,
    BISHOP,
    KNIGHT,
    ENVOY,
    MAGE,
    CATAPAULT, 
    LANCER
} pieceType;

typedef struct Piece
{
    float pieceX, pieceY; // world position
    int col, row;         // grid position (NEW)
    int dx;
    int dy;
    int owner; // 1 = p1, 2 = p2
    bool active;
    pieceType type;
    bool moved;
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
bool pieceSpawnable(AppState *app, int player);
bool inTerritory(AppState *app, int row, int col);
int pieceCost(pieceType type);

#endif