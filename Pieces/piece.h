#ifndef PIECES_H
#define PIECES_H

#define MAX_PIECES 32

#include <SDL3/SDL.h>

typedef struct AppState AppState;

typedef enum {
    PIECE_KING,
    PIECE_KNIGHT,
    PIECE_PAWN
} PieceType;

typedef struct Piece {
    int pieceX, pieceY;
    bool active;
    PieceType type;
} Piece;

SDL_FRect GetPieceRect(Piece *p, int cellSize);

void spawnPiece(AppState *app);
void renderPiece(AppState *app);

#endif