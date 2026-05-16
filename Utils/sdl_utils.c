#include "sdl_utils.h"
#include <stdio.h>
#include "../game.h"

static SDL_AudioDeviceID audio_device = 0;

SDL_Window *sdl_initialize_window(const char *app_name, int app_width, int app_height)
{
    SDL_Window *newWindow = NULL;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return NULL;
    }

    newWindow = SDL_CreateWindow(
        app_name,
        app_width,
        app_height,
        SDL_WINDOW_RESIZABLE);

    if (!newWindow)
    {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    SDL_SetWindowPosition(newWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    return newWindow;
}

SDL_Renderer *sdl_initialize_renderer(SDL_Window *window)
{
    SDL_Renderer *newRenderer = SDL_CreateRenderer(window, NULL);
    if (!newRenderer)
    {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return newRenderer;
}

int sdl_initialize_audio()
{
    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audio_device == 0)
    {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return audio_device;
}

bool init_sound(const char *fname, Sound *sound)
{
    bool retval = false;
    SDL_AudioSpec spec;
    char *wav_path = NULL;

    /* Load the .wav files from wherever the app is being run from. */
    printf("Loading sound from file: %s%s\n", SDL_GetBasePath(), fname);
    SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), fname); /* allocate a string of the full file path */
    if (!SDL_LoadWAV(wav_path, &spec, &sound->wav_data, &sound->wav_data_len))
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return false;
    }

    /* Create an audio stream. Set the source format to the wav's format (what
       we'll input), leave the dest format NULL here (it'll change to what the
       device wants once we bind it). */
    sound->stream = SDL_CreateAudioStream(&spec, NULL);
    if (!sound->stream)
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    }
    else if (!SDL_BindAudioStream(audio_device, sound->stream))
    { /* once bound, it'll start playing when there is data available! */
        SDL_Log("Failed to bind '%s' stream to device: %s", fname, SDL_GetError());
    }
    else
    {
        retval = true; /* success! */
    }

    SDL_free(wav_path); /* done with this string. */
    return retval;
}

void playSound(Sound *sound)
{
    SDL_ClearAudioStream(sound->stream);
    if (SDL_GetAudioStreamQueued(sound->stream) < ((int)sound->wav_data_len))
        SDL_PutAudioStreamData(sound->stream, sound->wav_data, (int)sound->wav_data_len);
}

void stopSound(Sound *sound)
{
    SDL_ClearAudioStream(sound->stream);
}

SDL_Texture *sdl_load_texture(SDL_Renderer *renderer, const char *file_path)
{
    if (!renderer)
    {
        SDL_Log("sdl_load_texture: renderer is NULL");
        return NULL;
    }

    char *full_path = NULL;
    printf("Loading texture from file: %s%s\n", SDL_GetBasePath(), file_path);
    if (SDL_asprintf(&full_path, "%s%s", SDL_GetBasePath(), file_path) < 0)
    {
        SDL_Log("sdl_load_texture: SDL_asprintf failed");
        return NULL;
    }

    SDL_Texture *tex = IMG_LoadTexture(renderer, full_path);
    SDL_free(full_path);

    if (!tex)
    {
        SDL_Log("IMG_LoadTexture('%s') error: %s", file_path, SDL_GetError());
        return NULL;
    }

    return tex;
}

void showText(SDL_Renderer *renderer, int x, int y, const char *text, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDebugText(renderer, x, y, text);
}

SDL_Texture* LoadTexture(SDL_Renderer *renderer, const char *path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        SDL_Log("ERROR: Failed to load texture at %s: %s", path, SDL_GetError());
    } else {
        SDL_Log("SUCCESS: Loaded texture: %s", path);
    }
    return texture;
}

bool LoadAllGameTextures(AppState *app) {
    // 1. Define a struct to map your pointers to file paths
    typedef struct {
        SDL_Texture **target;
        const char *path;
    } TextureRegistry;

    TextureRegistry registry[] = {
        // Tiles
        {&app->grassTexture, "Assets/tiles/grass.png"},
        {&app->waterTexture, "Assets/tiles/water.png"},
        {&app->bridgeTopTexture, "Assets/tiles/bridgeTop.png"},
        {&app->bridgeBottomTexture, "Assets/tiles/bridgeBottom.png"},
        {&app->treeTexture, "Assets/tiles/trees.png"},
        {&app->treeLeftTexture, "Assets/tiles/treesLeft.png"},
        {&app->treeRightTexture, "Assets/tiles/treesRight.png"},
        {&app->townTexture, "Assets/tiles/town.png"},
        {&app->upgradePlatformTexture, "Assets/tiles/upgradePlat.png"},

        // Shores
        {&app->shoreLeftTexture, "Assets/tiles/shore/shoreLeft.png"},
        {&app->shoreRightTexture, "Assets/tiles/shore/shoreRight.png"},
        {&app->shoreTopTexture, "Assets/tiles/shore/shoreTop.png"},
        {&app->shoreBottomTexture, "Assets/tiles/shore/shoreBottom.png"},
        {&app->shoreTopLeftTexture, "Assets/tiles/shore/shoreTopLeft.png"},
        {&app->shoreBottomLeftTexture, "Assets/tiles/shore/shoreBottomLeft.png"},
        {&app->shoreTopRightTexture, "Assets/tiles/shore/shoreTopRight.png"},
        {&app->shoreBottomRightTexture, "Assets/tiles/shore/shoreBottomRight.png"},
        {&app->shoreCornerTopLeftTexture, "Assets/tiles/shore/shoreCornerTopLeft.png"},
        {&app->shoreCornerBottomLeftTexture, "Assets/tiles/shore/shoreCornerBottomLeft.png"},
        {&app->shoreCornerTopRightTexture, "Assets/tiles/shore/shoreCornerTopRight.png"},
        {&app->shoreCornerBottomRightTexture, "Assets/tiles/shore/shoreCornerBottomRight.png"},

        // UI & Logo
        {&app->logoTexture, "Assets/images/regnaraLogo.png"},
        {&app->buttonOn, "Assets/UI/menus/buttonOn.png"},
        {&app->buttonHovered, "Assets/UI/menus/buttonOff.png"},
        {&app->blueGoldTexture, "Assets/UI/inRound/goldBlue.png"},
        {&app->blueTownsTexture, "Assets/UI/inRound/townsBlue.png"},
        {&app->redGoldTexture, "Assets/UI/inRound/goldRed.png"},
        {&app->redTownsTexture, "Assets/UI/inRound/townsRed.png"},

        // Blue Pieces
        {&app->bluePawnTexture, "Assets/pieces/blue/pawn.png"},
        {&app->blueKnightTexture, "Assets/pieces/blue/knight.png"},
        {&app->blueBishopTexture, "Assets/pieces/blue/bishop.png"},
        {&app->blueRookTexture, "Assets/pieces/blue/rook.png"},
        {&app->blueQueenTexture, "Assets/pieces/blue/queen.png"},
        {&app->blueKingTexture, "Assets/pieces/blue/king.png"},
        {&app->blueEnvoyTexture,    "Assets/pieces/blue/envoy.png"},
        {&app->blueLancerTexture,   "Assets/pieces/blue/lancer.png"},
        {&app->blueMageTexture,     "Assets/pieces/blue/mage.png"},
        {&app->blueCatapultTexture, "Assets/pieces/blue/catapult.png"},

        // Red Pieces
        {&app->redPawnTexture, "Assets/pieces/red/pawn.png"},
        {&app->redKnightTexture, "Assets/pieces/red/knight.png"},
        {&app->redBihsopTexture, "Assets/pieces/red/bishop.png"},
        {&app->redRookTexture, "Assets/pieces/red/rook.png"},
        {&app->redQueenTexture, "Assets/pieces/red/queen.png"},
        {&app->redKingTexture, "Assets/pieces/red/king.png"},
        {&app->redEnvoyTexture,     "Assets/pieces/red/envoy.png"},
        {&app->redLancerTexture,    "Assets/pieces/red/lancer.png"},
        {&app->redMageTexture,      "Assets/pieces/red/mage.png"},
        {&app->redCatapultTexture,  "Assets/pieces/red/catapult.png"},

        // Moves
        {&app->movePossible, "Assets/images/moves/possible.png"},
        {&app->moveCapture, "Assets/images/moves/impossible.png"},
        {&app->moveRanged,  "Assets/images/moves/ranged.png"},
        {&app->moveLancer,  "Assets/images/moves/lancer.png"}
    };

    int totalTextures = sizeof(registry) / sizeof(TextureRegistry);

    for (int i = 0; i < totalTextures; i++) {
        *registry[i].target = LoadTexture(app->renderer, registry[i].path);
        
        // 2. Immediate Centralized Error Check
        if (*registry[i].target == NULL) {
            SDL_Log("CRITICAL ERROR: Failed to load %s", registry[i].path);
            return false; // Stop initialization if any core asset is missing
        }
    }

    return true; // All textures loaded successfully
}

void CleanupAllTextures(AppState *app) {
    // Use an array of pointers to your textures
    SDL_Texture* allTextures[] = {
        app->grassTexture, app->waterTexture, app->bridgeTopTexture, 
        app->bridgeBottomTexture, app->treeTexture, app->treeLeftTexture, 
        app->treeRightTexture, app->townTexture, app->upgradePlatformTexture,
        app->shoreLeftTexture, app->shoreRightTexture, app->shoreTopTexture, 
        app->shoreBottomTexture, app->shoreTopLeftTexture, app->shoreBottomLeftTexture, 
        app->shoreTopRightTexture, app->shoreBottomRightTexture, app->shoreCornerTopLeftTexture, 
        app->shoreCornerBottomLeftTexture, app->shoreCornerTopRightTexture, app->shoreCornerBottomRightTexture,
        app->logoTexture, app->buttonOn, app->buttonHovered, 
        app->blueGoldTexture, app->blueTownsTexture, app->redGoldTexture, app->redTownsTexture,
        app->bluePawnTexture, app->blueKnightTexture, app->blueBishopTexture, 
        app->blueRookTexture, app->blueQueenTexture, app->blueKingTexture,
        app->blueEnvoyTexture, app->blueLancerTexture, app->blueMageTexture, app->blueCatapultTexture,
        app->redPawnTexture, app->redKnightTexture, app->redBihsopTexture, 
        app->redRookTexture, app->redQueenTexture, app->redKingTexture,
        app->redEnvoyTexture, app->redLancerTexture, app->redMageTexture, app->redCatapultTexture,
        app->movePossible, app->moveCapture, app->moveRanged, app->moveLancer, app->possibleMovesTexture
    };

    int count = sizeof(allTextures) / sizeof(SDL_Texture*);
    for (int i = 0; i < count; i++) {
        if (allTextures[i]) SDL_DestroyTexture(allTextures[i]);
    }
}