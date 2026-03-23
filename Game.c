#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Utils/sdl_utils.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdlib.h>

//#include "tech_Tree/techTree.h"
#include "map/map.h"
//#include "camera/camera.h"

//definitions
#define APP_NAME "Regnara"
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

//texture initialization

//sound initialization


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
    SDL_Texture *playerTexture;
    SDL_Texture *collectibleTexture;
    InputState input;
    Uint64 lastTicksMS;
    float dt;
    //Player *player;
    float cameraX, cameraY;

    //GameState gameState;
    float timeLeft;
    int score;

    //Collectibles rings[20];
    float spawnTImer;
    int activeCount;// how many in world

} AppState;


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
    if (!SDL_CreateWindowAndRenderer("My Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app->window, &app->renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


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

    SDL_RenderPresent(app->renderer);
    // reset edge flagsW
    Input_BeginFrame(&app->input);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)result;
    AppState *app = (AppState *)appstate;
    if (!app)
        return;
    if (app->playerTexture)
        SDL_DestroyTexture(app->playerTexture);
    if (app->collectibleTexture)
        SDL_DestroyTexture(app->collectibleTexture);
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