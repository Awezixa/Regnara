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

void townCaptured(AppState *app) {
    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];
        bool currentKingPresent = false;

        // 1. Check if the CURRENT player's King is on this town
        for (int i = 0; i < MAX_PIECES; i++) {
            Piece *p = &app->pieces[i];
            
            // We assume pieces spawned during Player 1's turn are "his" 
            // If you don't have a p->owner field, we check if he's the active player
            if (p->active && p->type == KING && p->row == town->row && p->col == town->col) {
                // Verification: Is this piece actually owned by the current player?
                // For now, we assume any King found on the current player's turn is theirs
                currentKingPresent = true;
                break;
            }
        }

        // 2. Logic: If current player's King is here and they don't own it
        if (currentKingPresent && town->owner != app->currentPlayer) {
            town->captureTurns++;
            
            if (town->captureTurns >= 1) { // 1 turn to capture
                int oldOwner = town->owner;
                town->owner = app->currentPlayer;
                town->captureTurns = 0;

                // 3. Update persistent counts
                if (app->currentPlayer == 1) app->P1.towns++;
                else if (app->currentPlayer == 2) app->P2.towns++;

                // Subtract from previous owner
                if (oldOwner == 1) app->P1.towns--;
                else if (oldOwner == 2) app->P2.towns--;
            }
        } else {
            // 4. Reset if King leaves or if it's the other player's turn
            town->captureTurns = 0;
        }
    }
}

void drawTerritory(AppState *app) {
    // Set the blend mode once for the whole loop
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    for (int t = 0; t < app->tTowns; t++) {
        Town *town = &app->towns[t];

        if (town->owner != 0) {
            // Set Color: Blue for P1, Red for P2
            if (town->owner == 1) {
                SDL_SetRenderDrawColor(app->renderer, 0, 100, 255, 80); 
            } else {
                SDL_SetRenderDrawColor(app->renderer, 255, 50, 50, 80);
            }

            // Loop through the 3x3 radius (-1, 0, 1)
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    // CRITICAL: Offset from the actual town position
                    int r = town->row + dr;
                    int c = town->col + dc;

                    // 3. Bounds check to ensure we stay on the map
                    if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS) {
                        // 4. Convert World Grid to Screen Rect
                        SDL_FRect tileRect = camera2d_world_to_screen_rect(
                            &app->camera,
                            (float)(c * TILE_SIZE),
                            (float)(r * TILE_SIZE),
                            (float)TILE_SIZE,
                            (float)TILE_SIZE
                        );

                        // 5. Fill the tile
                        SDL_RenderFillRect(app->renderer, &tileRect);
                    }
                }
            }
        }
    }
}