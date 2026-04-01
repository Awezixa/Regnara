#include "piece.h"
#include "../game.h"

void spawnPiece(AppState *app){

    //change made to convert mouse screen position to world mouse position, to prevent pieces spawning wrong place

    if (app->input.mouseLeftPressed && app->pieceCount < MAX_PIECES){

        for (int i = 0; i < MAX_PIECES; i++)
        {
            //need to add check for map data to only allow spawn towns and area around towns
            //char tile = map_data[row][col];
            if(!app->pieces[i].active){
                // CONVERT SCREEN TO WORLD: 
                // Math: (ScreenPos / Zoom) + CameraOffset
                app->pieces[i].pieceX = (app->input.mouseX/ app->camera.zoom) + app->camera.x;
                app->pieces[i].pieceY = (app->input.mouseY/ app->camera.zoom) + app->camera.y;

                app->pieces[i].active = true;
                app->pieceCount++;
                break;
            }
        }
    }
}

void renderPiece(AppState *app) {
    for (int i = 0; i < MAX_PIECES; i++) {
        if (app->pieces[i].active) {
            // Convert the stored world coordinates back to screen pixels for drawing
            SDL_FRect p = camera2d_world_to_screen_rect(
                &app->camera, 
                app->pieces[i].pieceX, 
                app->pieces[i].pieceY, 
                32.0f, 32.0f // World size of the piece
            );

            SDL_RenderTexture(app->renderer, app->pawnTexture, NULL, &p);
        }
    }
}
