#ifndef MOVES_H
#define MOVES_H

#include "../game.h"
#include "piece.h"

void GenerateMoves(AppState *app, Piece *p);
void GenerateKingMoves(AppState *app, Piece *p);
void GenerateKnightMoves(AppState *app, Piece *p);
void GenerateBishopMoves(AppState *app, Piece *p);
void GenerateRookMoves(AppState *app, Piece *p);
void GenerateQueenMoves(AppState *app, Piece *p);
void GenerateLancerMoves(AppState *app, Piece *p);
void GenerateMageMoves(AppState *app, Piece *p);
void GenerateCatapultMoves(AppState *app, Piece *p);

void LancerAttack(AppState *app, Piece *attacker, int row, int col);
void CatapultAttack(AppState *app, int row, int col);

void GenerateMageAttacks(AppState *app, Piece *p);
void GenerateCatapultAttacks(AppState *app, Piece *p);

void PossibleMovesShow(AppState *app, Piece *p);
bool isTileOccupied(AppState *app, int row, int col);
void CapturePiece(AppState *app, int row, int col);

Piece* GetPieceAt(AppState *app, int row, int col);

#endif