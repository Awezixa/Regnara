#ifndef MOVES_H
#define MOVES_H

typedef struct AppState AppState;
typedef struct Piece Piece;

void GenerateMoves(AppState *app, Piece *p);

#endif