#include "../game.h"
#include "map.h"
#include <stdio.h>


char map_data[MAP_ROWS][MAP_COLS];

/*
can make map in Tiled then export as CSV.
replace commas with spaces and then associate csv ids to what you want and then load map that way
*/

// Xavier with Cesar's help
void loadMap(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Failed to open map file");
        return;
    }

    printf("loading map %s", filename);
    for (int row = 0; row < MAP_ROWS; row++)
    {
        for (int col = 0; col < MAP_COLS; col++)
        {
            int ch = fgetc(file);
            if (ch == EOF)
            {
                fclose(file);
                return;
            }
            if (ch == '\n')
            {
                col--;
                continue;
            }
            map_data[row][col] = (char)ch;
        }
    }
    fclose(file);
}

//made with Gemini (Xavier), ask Cesar if it ok
void renderMap(SDL_Renderer *renderer, AppState *app) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_FRect destRect = camera2d_world_to_screen_rect(
                &app->camera,
                (float)(col * TILE_SIZE), 
                (float)(row * TILE_SIZE), 
                (float)TILE_SIZE, 
                (float)TILE_SIZE
            );

            SDL_Texture *targetTexture = NULL;

            // Map the character in the txt file to the loaded texture
            switch (map_data[row][col]) {
                case 'G': targetTexture = app->grassTexture; break;
                case 'W': targetTexture = app->waterTexture; break;
                case 'B': targetTexture = app->bridgeTexture; break;
                case 'T': targetTexture = app->townTexture; break;
                case 'S': targetTexture = app->castleTexture; break;
                default: break; 
            }

            if (targetTexture) {
                SDL_RenderTexture(renderer, targetTexture, NULL, &destRect);
            }

            //to create grid texture to map
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
            SDL_RenderRect(renderer, &destRect);
        }
    }
}


int townCount(void){
    int total = 0;
    for(int row = 0; row < MAP_ROWS; row++){
        for (int col = 0; col < MAP_COLS; col++){
           if (map_data[row][col] == 'T') {
                total++;
           } 
        }
    }
    return total;
}


void initTowns(AppState *app){
    app->tTowns = 0;
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (map_data[r][c] == 'T') {
                app->towns[app->tTowns].row = r;
                app->towns[app->tTowns].col = c;
                app->towns[app->tTowns].owner = 0; // Neutral
                app->towns[app->tTowns].captureTurns = 0;
                app->tTowns++;
            }
        }
    }
}

void townCaptured(AppState *app){
    for (int t = 0; t < app->tTowns; t++)
    {
        Town *town = &app->towns[t];
        bool kingPresent = false;
        int kingOwner = 0;

        for (int i = 0; i < MAX_PIECES; i++) {
            Piece *p = &app->pieces[i];
            if (p->active && p->type == KING && p->row == town->row && p->col == town->col) {
                kingPresent = true;
                
                // Determine King's player based on turn (adjust if you have a p->owner field)
                kingOwner = (app->turnCounter % 2 == 1) ? 1 : 2; 
                break;
            }
        }
        // 3. Update capture progress
        if (kingPresent && town->owner != kingOwner) {
            town->captureTurns++;
            app->P1.towns++;
            // If they've held it for 1 turn, it changes ownership
            if (town->captureTurns >= 1) {
                town->owner = kingOwner;
                town->captureTurns = 0; // Reset progress once captured
            }
        } else {
            // Reset progress if the King leaves before the turn ends
            town->captureTurns = 0;
        }
    }
    
}