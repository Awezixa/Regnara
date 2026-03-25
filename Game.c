#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

//file includes
#include "game.h"

//other functions (to be moved elsewhere)
void drawMenu(AppState *app);

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
    if (!SDL_CreateWindowAndRenderer(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app->window, &app->renderer))
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
    GameState *gamestate = &app->gameState;

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
    loadMap("map/maps/map1.txt");

    //gamestate swaps
    switch (app->gameState)
    {
    case STATE_MENU:
        // draw menu first
        //drawMenu(app);
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
    //reset edge flags
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
