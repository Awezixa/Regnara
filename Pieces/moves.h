#ifndef MOVES_H
#define MOVES_H

#include "../game.h"
#include "piece.h"

void GenerateMoves(AppState *app, Piece *p);
void GenerateKingMoves(AppState *app, Piece *p);

#endif