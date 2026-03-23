#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils/sdl_utils.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gameStates/gameState.h"
#include "map/map.h"
//#include "camera/camera.h"

//definitions
#define APP_NAME "Regnara"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TILE_SIZE 32

//can move to other files later
typedef struct InputState
{
    float mouseX;
    float mouseY;
    bool mouseLeftDown;
    bool mouseLeftPressed;
    bool mouseLeftReleased;
    bool keyDown[SDL_SCANCODE_COUNT];
    bool keyPressed[SDL_SCANCODE_COUNT];
    bool keyReleased[SDL_SCANCODE_COUNT];
} InputState;

typedef struct AppState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    InputState input;
    Uint64 lastTicksMS;
    float dt;
    GameState gameState;

    //temporary texture loading
    SDL_Texture *grassTexture;
    SDL_Texture *waterTexture;
    SDL_Texture *bridgeTexture;
    SDL_Texture *logoTexture;


} AppState;

//other functions (to be moved elsewhere)
void renderMap(SDL_Renderer *renderer, AppState *app);


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void)argc;
    (void)argv; // unused

    AppState *app = (AppState *)SDL_calloc(1, sizeof(AppState));
    
    if (!app)
    {
        SDL_Log("Out of memory.");
        return SDL_APP_FAILURE;
    }
    *appstate = app;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // create window and renderer
    if (!SDL_CreateWindowAndRenderer("Regnara", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app->window, &app->renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //font loading
    app->font = TTF_OpenFont("FORCED SQUARE.ttf", 30);
    if (!app->font)
    {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //texture loading
    app->grassTexture = IMG_LoadTexture(app->renderer, "Assets/images/grassTile.png");
    if (!app->grassTexture) {
        SDL_Log("IMG_LoadTexture failed for grass: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    app->waterTexture = IMG_LoadTexture(app->renderer, "Assets/images/waterTile.png");
    if (!app->waterTexture) {
        SDL_Log("IMG_LoadTexture failed for water: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    app->bridgeTexture = IMG_LoadTexture(app->renderer, "Assets/images/bridgeTile.png");
    if (!app->bridgeTexture) {
        SDL_Log("IMG_LoadTexture failed for bridge: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    app->logoTexture = IMG_LoadTexture(app->renderer, "Assets/images/Rengara_Logo.png");
    if (!app->logoTexture) {
        SDL_Log("IMG_LoadTexture failed for logo: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //other game initializations
    app->gameState = STATE_MENU;

    app->lastTicksMS = SDL_GetTicks();
    app->dt = 0.0f;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app = (AppState *)appstate;
    InputState *in = &app->input;
    //GameState *gamestate = &app->gameState;

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_Scancode sc = event->key.scancode;
        bool repeat = event->key.repeat != 0;
        if (sc >= 0 && sc < SDL_SCANCODE_COUNT)
        {
            if (!repeat && !in->keyDown[sc])
                in->keyPressed[sc] = true;
            in->keyDown[sc] = true;
        }
    }
    break;
    case SDL_EVENT_KEY_UP:
    {
        SDL_Scancode sc = event->key.scancode;
        if (sc >= 0 && sc < SDL_SCANCODE_COUNT)
        {
            in->keyDown[sc] = false;
            in->keyReleased[sc] = true;
        }
    }
    break;
    case SDL_EVENT_MOUSE_MOTION:
        in->mouseX = event->motion.x;
        in->mouseY = event->motion.y;
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (!in->mouseLeftDown)
                in->mouseLeftPressed = true;
            in->mouseLeftDown = true;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            in->mouseLeftDown = false;
            in->mouseLeftReleased = true;
        }
        break;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

//input resets for mouse and keys
void Input_BeginFrame(InputState *input)
{
    input->mouseLeftPressed = false;
    input->mouseLeftReleased = false;

    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        input->keyPressed[i] = false;
        input->keyReleased[i] = false;
    }
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = (AppState *)appstate;

    // compute dt
    Uint64 nowMS = SDL_GetTicks();
    Uint64 elapsedMS = nowMS - app->lastTicksMS;
    app->lastTicksMS = nowMS;

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    
    /*
        to load map, add to Appiterate
        loadMap("map/maps/map1.txt");
    */

    //gamestate swaps
    switch (app->gameState)
    {
    case STATE_MENU:
        // draw menu first
        // drawMenu(app);
        // if (app->input.keyPressed[SDL_SCANCODE_SPACE])
        // {
            
        //     app->gameState = STATE_PLAYING;
        // }
        break;
    case STATE_PLAYING:
        // updateGameLogic(app);, add similar function to this for movement and other things that will be used
        break;

    case STATE_END:
        // drawEndScreen(app);
        // if (app->input.keyPressed[SDL_SCANCODE_RETURN])
        // {
        //     app->gameState = STATE_MENU;
        // }
        break;
    default:
        break;
    }


    renderMap(app->renderer, app);
    SDL_RenderPresent(app->renderer);
    // reset edge flagsW
    Input_BeginFrame(&app->input);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //clean up
    (void)result;
    AppState *app = (AppState *)appstate;
    if (!app)
        return;
    //add function to destroy all textures
    if (app->grassTexture)
        SDL_DestroyTexture(app->grassTexture);
    if (app->waterTexture)
        SDL_DestroyTexture(app->waterTexture);
    if (app->bridgeTexture)
        SDL_DestroyTexture(app->bridgeTexture);
    if (app->font)
        TTF_CloseFont(app->font);
    if (app->renderer)
        SDL_DestroyRenderer(app->renderer);
    if (app->window)
        SDL_DestroyWindow(app->window);
    TTF_Quit();
    SDL_Quit();
    SDL_free(app);
}

//made with Gemini (Xavier), ask Cesar if it ok
void renderMap(SDL_Renderer *renderer, AppState *app) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_FRect destRect = {
                (float)(col * TILE_SIZE), 
                (float)(row * TILE_SIZE), 
                (float)TILE_SIZE, 
                (float)TILE_SIZE
            };

            SDL_Texture *targetTexture = NULL;

            // Map the character in the txt file to the loaded texture
            switch (map_data[row][col]) {
                case 'G': targetTexture = app->grassTexture; break;
                case 'W': targetTexture = app->waterTexture; break;
                case 'B': targetTexture = app->bridgeTexture; break;
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