#ifndef PIECE_H
#define PIECE_H

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
void spawnPiece(AppState *app);
void renderPiece(AppState *app);


#endif