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
        {&app->blueInfoBox, "Assets/UI/inRound/goldBlue.png"},
        {&app->redInfoBox, "Assets/UI/inRound/goldRed.png"},
        {&app->techTreeBackground, "Assets/techTree/techTreeBackground.png"},

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

        {&app->bluePawnUsedTexture, "Assets/pieces/blueUsed/pawn.png"},
        {&app->blueKnightUsedTexture, "Assets/pieces/blueUsed/knight.png"},
        {&app->blueBishopUsedTexture, "Assets/pieces/blueUsed/bishop.png"},
        {&app->blueRookUsedTexture, "Assets/pieces/blueUsed/rook.png"},
        {&app->blueQueenUsedTexture, "Assets/pieces/blueUsed/queen.png"},
        {&app->blueKingUsedTexture, "Assets/pieces/blueUsed/king.png"},
        {&app->blueEnvoyUsedTexture,    "Assets/pieces/blueUsed/envoy.png"},
        {&app->blueLancerUsedTexture,   "Assets/pieces/blueUsed/lancer.png"},
        {&app->blueMageUsedTexture,     "Assets/pieces/blueUsed/mage.png"},
        {&app->blueCatapultUsedTexture, "Assets/pieces/blueUsed/catapult.png"},

        // Red Pieces
        {&app->redPawnTexture, "Assets/pieces/red/pawn.png"},
        {&app->redKnightTexture, "Assets/pieces/red/knight.png"},
        {&app->redBishopTexture, "Assets/pieces/red/bishop.png"},
        {&app->redRookTexture, "Assets/pieces/red/rook.png"},
        {&app->redQueenTexture, "Assets/pieces/red/queen.png"},
        {&app->redKingTexture, "Assets/pieces/red/king.png"},
        {&app->redEnvoyTexture,     "Assets/pieces/red/envoy.png"},
        {&app->redLancerTexture,    "Assets/pieces/red/lancer.png"},
        {&app->redMageTexture,      "Assets/pieces/red/mage.png"},
        {&app->redCatapultTexture,  "Assets/pieces/red/catapult.png"},

        {&app->redPawnUsedTexture, "Assets/pieces/redUsed/pawn.png"},
        {&app->redKnightUsedTexture, "Assets/pieces/redUsed/knight.png"},
        {&app->redBishopUsedTexture, "Assets/pieces/redUsed/bishop.png"},
        {&app->redRookUsedTexture, "Assets/pieces/redUsed/rook.png"},
        {&app->redQueenUsedTexture, "Assets/pieces/redUsed/queen.png"},
        {&app->redKingUsedTexture, "Assets/pieces/redUsed/king.png"},
        {&app->redEnvoyUsedTexture,     "Assets/pieces/redUsed/envoy.png"},
        {&app->redLancerUsedTexture,    "Assets/pieces/redUsed/lancer.png"},
        {&app->redMageUsedTexture,      "Assets/pieces/redUsed/mage.png"},
        {&app->redCatapultUsedTexture,  "Assets/pieces/redUsed/catapult.png"},

        // Moves
        {&app->movePossible, "Assets/images/moves/possible.png"},
        {&app->moveCapture, "Assets/images/moves/impossible.png"},
        {&app->moveRanged,  "Assets/images/moves/ranged.png"},
        {&app->moveLancer,  "Assets/images/moves/lancer.png"},

        //techTreeBlueLocked

        {&app->techTreeBluePawnLocked,  "Assets/techTree/blue/locked/pawn.png"},
        {&app->techTreeBlueKnightLocked,  "Assets/techTree/blue/locked/knight.png"},
        {&app->techTreeBlueBishopLocked,  "Assets/techTree/blue/locked/bishop.png"},
        {&app->techTreeBlueRookLocked,  "Assets/techTree/blue/locked/rook.png"},
        {&app->techTreeBlueQueenLocked,  "Assets/techTree/blue/locked/queen.png"},
        {&app->techTreeBlueEnvoyLocked,  "Assets/techTree/blue/locked/envoy.png"},
        {&app->techTreeBlueLancerLocked,  "Assets/techTree/blue/locked/lancer.png"},
        {&app->techTreeBlueMageLocked,  "Assets/techTree/blue/locked/mage.png"},
        {&app->techTreeBlueCatapultLocked,  "Assets/techTree/blue/locked/catapult.png"},
        {&app->techTreeBlueGoldLocked,  "Assets/techTree/blue/locked/moreGold.png"},
        {&app->techTreeBluePiecesLocked,  "Assets/techTree/blue/locked/morePieces.png"},
        {&app->techTreeBluePlatformLocked,  "Assets/techTree/blue/locked/upgPlatform.png"},

        //techTreeBlueBuyable

        {&app->techTreeBluePawnBuyable,  "Assets/techTree/blue/buyable/pawn.png"},
        {&app->techTreeBlueKnightBuyable,  "Assets/techTree/blue/buyable/knight.png"},
        {&app->techTreeBlueBishopBuyable,  "Assets/techTree/blue/buyable/bishop.png"},
        {&app->techTreeBlueRookBuyable,  "Assets/techTree/blue/buyable/rook.png"},
        {&app->techTreeBlueQueenBuyable,  "Assets/techTree/blue/buyable/queen.png"},
        {&app->techTreeBlueEnvoyBuyable,  "Assets/techTree/blue/buyable/envoy.png"},
        {&app->techTreeBlueLancerBuyable,  "Assets/techTree/blue/buyable/lancer.png"},
        {&app->techTreeBlueMageBuyable,  "Assets/techTree/blue/buyable/mage.png"},
        {&app->techTreeBlueCatapultBuyable,  "Assets/techTree/blue/buyable/catapult.png"},
        {&app->techTreeBlueGoldBuyable,  "Assets/techTree/blue/buyable/moreGold.png"},
        {&app->techTreeBluePiecesBuyable,  "Assets/techTree/blue/buyable/morePieces.png"},
        {&app->techTreeBluePlatformBuyable,  "Assets/techTree/blue/buyable/upgPlatform.png"},

        //techTreeBlueBought

        {&app->techTreeBluePawnBought,  "Assets/techTree/blue/bought/pawn.png"},
        {&app->techTreeBlueKnightBought,  "Assets/techTree/blue/bought/knight.png"},
        {&app->techTreeBlueBishopBought,  "Assets/techTree/blue/bought/bishop.png"},
        {&app->techTreeBlueRookBought,  "Assets/techTree/blue/bought/rook.png"},
        {&app->techTreeBlueQueenBought,  "Assets/techTree/blue/bought/queen.png"},
        {&app->techTreeBlueEnvoyBought,  "Assets/techTree/blue/bought/envoy.png"},
        {&app->techTreeBlueLancerBought,  "Assets/techTree/blue/bought/lancer.png"},
        {&app->techTreeBlueMageBought,  "Assets/techTree/blue/bought/mage.png"},
        {&app->techTreeBlueCatapultBought,  "Assets/techTree/blue/bought/catapult.png"},
        {&app->techTreeBlueGoldBought,  "Assets/techTree/blue/bought/moreGold.png"},
        {&app->techTreeBluePiecesBought,  "Assets/techTree/blue/bought/morePieces.png"},
        {&app->techTreeBluePlatformBought,  "Assets/techTree/blue/bought/upgPlatform.png"},

        //techTreeRedLocked

        {&app->techTreeRedPawnLocked,  "Assets/techTree/red/locked/pawn.png"},
        {&app->techTreeRedKnightLocked,  "Assets/techTree/red/locked/knight.png"},
        {&app->techTreeRedBishopLocked,  "Assets/techTree/red/locked/bishop.png"},
        {&app->techTreeRedRookLocked,  "Assets/techTree/red/locked/rook.png"},
        {&app->techTreeRedQueenLocked,  "Assets/techTree/red/locked/queen.png"},
        {&app->techTreeRedEnvoyLocked,  "Assets/techTree/red/locked/envoy.png"},
        {&app->techTreeRedLancerLocked,  "Assets/techTree/red/locked/lancer.png"},
        {&app->techTreeRedMageLocked,  "Assets/techTree/red/locked/mage.png"},
        {&app->techTreeRedCatapultLocked,  "Assets/techTree/red/locked/catapult.png"},
        {&app->techTreeRedGoldLocked,  "Assets/techTree/red/locked/moreGold.png"},
        {&app->techTreeRedPiecesLocked,  "Assets/techTree/red/locked/morePieces.png"},
        {&app->techTreeRedPlatformLocked,  "Assets/techTree/red/locked/upgPlatform.png"},

        //techTreeRedBuyable

        {&app->techTreeRedPawnBuyable,  "Assets/techTree/red/buyable/pawn.png"},
        {&app->techTreeRedKnightBuyable,  "Assets/techTree/red/buyable/knight.png"},
        {&app->techTreeRedBishopBuyable,  "Assets/techTree/red/buyable/bishop.png"},
        {&app->techTreeRedRookBuyable,  "Assets/techTree/red/buyable/rook.png"},
        {&app->techTreeRedQueenBuyable,  "Assets/techTree/red/buyable/queen.png"},
        {&app->techTreeRedEnvoyBuyable,  "Assets/techTree/red/buyable/envoy.png"},
        {&app->techTreeRedLancerBuyable,  "Assets/techTree/red/buyable/lancer.png"},
        {&app->techTreeRedMageBuyable,  "Assets/techTree/red/buyable/mage.png"},
        {&app->techTreeRedCatapultBuyable,  "Assets/techTree/red/buyable/catapult.png"},
        {&app->techTreeRedGoldBuyable,  "Assets/techTree/red/buyable/moreGold.png"},
        {&app->techTreeRedPiecesBuyable,  "Assets/techTree/red/buyable/morePieces.png"},
        {&app->techTreeRedPlatformBuyable,  "Assets/techTree/red/buyable/upgPlatform.png"},

        //techTreeRedBought

        {&app->techTreeRedPawnBought,  "Assets/techTree/red/bought/pawn.png"},
        {&app->techTreeRedKnightBought,  "Assets/techTree/red/bought/knight.png"},
        {&app->techTreeRedBishopBought,  "Assets/techTree/red/bought/bishop.png"},
        {&app->techTreeRedRookBought,  "Assets/techTree/red/bought/rook.png"},
        {&app->techTreeRedQueenBought,  "Assets/techTree/red/bought/queen.png"},
        {&app->techTreeRedEnvoyBought,  "Assets/techTree/red/bought/envoy.png"},
        {&app->techTreeRedLancerBought,  "Assets/techTree/red/bought/lancer.png"},
        {&app->techTreeRedMageBought,  "Assets/techTree/red/bought/mage.png"},
        {&app->techTreeRedCatapultBought,  "Assets/techTree/red/bought/catapult.png"},
        {&app->techTreeRedGoldBought,  "Assets/techTree/red/bought/moreGold.png"},
        {&app->techTreeRedPiecesBought,  "Assets/techTree/red/bought/morePieces.png"},
        {&app->techTreeRedPlatformBought,  "Assets/techTree/red/bought/upgPlatform.png"},

        //UICardsBlueLocked

        {&app->UIBluePawnLocked,  "Assets/cards/blueCards/locked/pawn.png"},
        {&app->UIBlueKnightLocked,  "Assets/cards/blueCards/locked/knight.png"},
        {&app->UIBlueBishopLocked,  "Assets/cards/blueCards/locked/bishop.png"},
        {&app->UIBlueRookLocked,  "Assets/cards/blueCards/locked/rook.png"},
        {&app->UIBlueQueenLocked,  "Assets/cards/blueCards/locked/queen.png"},
        {&app->UIBlueEnvoyLocked,  "Assets/cards/blueCards/locked/envoy.png"},
        {&app->UIBlueLancerLocked,  "Assets/cards/blueCards/locked/lancer.png"},
        {&app->UIBlueMageLocked,  "Assets/cards/blueCards/locked/mage.png"},
        {&app->UIBlueCatapultLocked,  "Assets/cards/blueCards/locked/catapult.png"},

        //UICardsBlueAvailable

        {&app->UIBluePawnAvailable,  "Assets/cards/blueCards/available/pawn.png"},
        {&app->UIBlueKnightAvailable,  "Assets/cards/blueCards/available/knight.png"},
        {&app->UIBlueBishopAvailable,  "Assets/cards/blueCards/available/bishop.png"},
        {&app->UIBlueRookAvailable,  "Assets/cards/blueCards/available/rook.png"},
        {&app->UIBlueQueenAvailable,  "Assets/cards/blueCards/available/queen.png"},
        {&app->UIBlueEnvoyAvailable,  "Assets/cards/blueCards/available/envoy.png"},
        {&app->UIBlueLancerAvailable,  "Assets/cards/blueCards/available/lancer.png"},
        {&app->UIBlueMageAvailable,  "Assets/cards/blueCards/available/mage.png"},
        {&app->UIBlueCatapultAvailable,  "Assets/cards/blueCards/available/catapult.png"},

        //UICardsBlueUnavailable

        {&app->UIBluePawnUnavailable,  "Assets/cards/blueCards/unavailable/pawn.png"},
        {&app->UIBlueKnightUnavailable,  "Assets/cards/blueCards/unavailable/knight.png"},
        {&app->UIBlueBishopUnavailable,  "Assets/cards/blueCards/unavailable/bishop.png"},
        {&app->UIBlueRookUnavailable,  "Assets/cards/blueCards/unavailable/rook.png"},
        {&app->UIBlueQueenUnavailable,  "Assets/cards/blueCards/unavailable/queen.png"},
        {&app->UIBlueEnvoyUnavailable,  "Assets/cards/blueCards/unavailable/envoy.png"},
        {&app->UIBlueLancerUnavailable,  "Assets/cards/blueCards/unavailable/lancer.png"},
        {&app->UIBlueMageUnavailable,  "Assets/cards/blueCards/unavailable/mage.png"},
        {&app->UIBlueCatapultUnavailable,  "Assets/cards/blueCards/unavailable/catapult.png"},

        //UICardsRedLocked

        {&app->UIRedPawnLocked,  "Assets/cards/redCards/locked/pawn.png"},
        {&app->UIRedKnightLocked,  "Assets/cards/redCards/locked/knight.png"},
        {&app->UIRedBishopLocked,  "Assets/cards/redCards/locked/bishop.png"},
        {&app->UIRedRookLocked,  "Assets/cards/redCards/locked/rook.png"},
        {&app->UIRedQueenLocked,  "Assets/cards/redCards/locked/queen.png"},
        {&app->UIRedEnvoyLocked,  "Assets/cards/redCards/locked/envoy.png"},
        {&app->UIRedLancerLocked,  "Assets/cards/redCards/locked/lancer.png"},
        {&app->UIRedMageLocked,  "Assets/cards/redCards/locked/mage.png"},
        {&app->UIRedCatapultLocked,  "Assets/cards/redCards/locked/catapult.png"},

        //UICardsRedAvailable

        {&app->UIRedPawnAvailable,  "Assets/cards/redCards/available/pawn.png"},
        {&app->UIRedKnightAvailable,  "Assets/cards/redCards/available/knight.png"},
        {&app->UIRedBishopAvailable,  "Assets/cards/redCards/available/bishop.png"},
        {&app->UIRedRookAvailable,  "Assets/cards/redCards/available/rook.png"},
        {&app->UIRedQueenAvailable,  "Assets/cards/redCards/available/queen.png"},
        {&app->UIRedEnvoyAvailable,  "Assets/cards/redCards/available/envoy.png"},
        {&app->UIRedLancerAvailable,  "Assets/cards/redCards/available/lancer.png"},
        {&app->UIRedMageAvailable,  "Assets/cards/redCards/available/mage.png"},
        {&app->UIRedCatapultAvailable,  "Assets/cards/redCards/available/catapult.png"},

        //UICardsRedUnavailable

        {&app->UIRedPawnUnavailable,  "Assets/cards/redCards/unavailable/pawn.png"},
        {&app->UIRedKnightUnavailable,  "Assets/cards/redCards/unavailable/knight.png"},
        {&app->UIRedBishopUnavailable,  "Assets/cards/redCards/unavailable/bishop.png"},
        {&app->UIRedRookUnavailable,  "Assets/cards/redCards/unavailable/rook.png"},
        {&app->UIRedQueenUnavailable,  "Assets/cards/redCards/unavailable/queen.png"},
        {&app->UIRedEnvoyUnavailable,  "Assets/cards/redCards/unavailable/envoy.png"},
        {&app->UIRedLancerUnavailable,  "Assets/cards/redCards/unavailable/lancer.png"},
        {&app->UIRedMageUnavailable,  "Assets/cards/redCards/unavailable/mage.png"},
        {&app->UIRedCatapultUnavailable,  "Assets/cards/redCards/unavailable/catapult.png"},

        //UIOverlays

        {&app->UIRedOverlayAvailable,  "Assets/cards/redCards/overlays/availableOverlay.png"},
        {&app->UIBlueOverlayAvailable,  "Assets/cards/blueCards/overlays/availableOverlay.png"},
        {&app->UIRedOverlayUnavailable,  "Assets/cards/redCards/overlays/unavailableOverlay.png"},
        {&app->UIBlueOverlayUnavailable,  "Assets/cards/blueCards/overlays/unavailableOverlay.png"},

        //Upgrade Platforms
        
        {&app->UIBlueUpgPlatAvailable,  "Assets/cards/blueCards/available/upgPlat.png"},
        {&app->UIBlueUpgPlatLocked,  "Assets/cards/blueCards/locked/upgPlat.png"},
        {&app->UIRedUpgPlatLocked,  "Assets/cards/redCards/locked/upgPlat.png"},
        {&app->UIRedUpgPlatAvailable,  "Assets/cards/redCards/available/upgPlat.png"}
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
        app->blueInfoBox, app->blueInfoBox,
        app->bluePawnTexture, app->blueKnightTexture, app->blueBishopTexture, 
        app->blueRookTexture, app->blueQueenTexture, app->blueKingTexture,
        app->blueEnvoyTexture, app->blueLancerTexture, app->blueMageTexture, app->blueCatapultTexture,
        app->redPawnTexture, app->redKnightTexture, app->redBishopTexture, 
        app->redRookTexture, app->redQueenTexture, app->redKingTexture,
        app->redEnvoyTexture, app->redLancerTexture, app->redMageTexture, app->redCatapultTexture,
        app->movePossible, app->moveCapture, app->moveRanged, app->moveLancer, app->possibleMovesTexture,
        app->bluePawnUsedTexture, app->blueKnightUsedTexture, app->blueBishopUsedTexture, app->blueRookUsedTexture,
        app->blueQueenUsedTexture, app->blueKingUsedTexture, app->blueEnvoyUsedTexture, app->blueLancerUsedTexture,
        app->blueMageUsedTexture, app->blueCatapultUsedTexture,
        app->redPawnUsedTexture, app->redKnightUsedTexture, app->redBishopUsedTexture, app->redRookUsedTexture,
        app->redQueenUsedTexture, app->redKingUsedTexture, app->redEnvoyUsedTexture, app->redLancerUsedTexture,
        app->redMageUsedTexture, app->redCatapultUsedTexture,
        app->techTreeBackground,
        app->UIBluePawnLocked, app->UIBlueKnightLocked, app->UIBlueBishopLocked, app->UIBlueRookLocked,
        app->UIBlueQueenLocked, app->UIBlueKingLocked, app->UIBlueEnvoyLocked, app->UIBlueLancerLocked,
        app->UIBlueMageLocked, app->UIBlueCatapultLocked,
        app->UIRedPawnLocked, app->UIRedKnightLocked, app->UIRedBishopLocked, app->UIRedRookLocked,
        app->UIRedQueenLocked, app->UIRedKingLocked, app->UIRedEnvoyLocked, app->UIRedLancerLocked,
        app->UIRedMageLocked, app->UIRedCatapultLocked,
        app->UIBluePawnAvailable, app->UIBlueKnightAvailable, app->UIBlueBishopAvailable, app->UIBlueRookAvailable,
        app->UIBlueQueenAvailable, app->UIBlueKingAvailable, app->UIBlueEnvoyAvailable, app->UIBlueLancerAvailable,
        app->UIBlueMageAvailable, app->UIBlueCatapultAvailable,
        app->UIRedPawnAvailable, app->UIRedKnightAvailable, app->UIRedBishopAvailable, app->UIRedRookAvailable,
        app->UIRedQueenAvailable, app->UIRedKingAvailable, app->UIRedEnvoyAvailable, app->UIRedLancerAvailable,
        app->UIRedMageAvailable, app->UIRedCatapultAvailable,
        app->UIBluePawnUnavailable, app->UIBlueKnightUnavailable, app->UIBlueBishopUnavailable, app->UIBlueRookUnavailable,
        app->UIBlueQueenUnavailable, app->UIBlueKingUnavailable, app->UIBlueEnvoyUnavailable, app->UIBlueLancerUnavailable,
        app->UIBlueMageUnavailable, app->UIBlueCatapultUnavailable,
        app->UIRedPawnUnavailable, app->UIRedKnightUnavailable, app->UIRedBishopUnavailable, app->UIRedRookUnavailable,
        app->UIRedQueenUnavailable, app->UIRedKingUnavailable, app->UIRedEnvoyUnavailable, app->UIRedLancerUnavailable,
        app->UIRedMageUnavailable, app->UIRedCatapultUnavailable,
        app->techTreeBluePawnLocked, app->techTreeBlueKnightLocked, app->techTreeBlueBishopLocked, app->techTreeBlueRookLocked,
        app->techTreeBlueQueenLocked, app->techTreeBlueEnvoyLocked, app->techTreeBlueLancerLocked, app->techTreeBlueMageLocked,
        app->techTreeBlueCatapultLocked, app->techTreeBlueGoldLocked, app->techTreeBluePiecesLocked, app->techTreeBluePlatformLocked,
        app->techTreeRedPawnLocked, app->techTreeRedKnightLocked, app->techTreeRedBishopLocked, app->techTreeRedRookLocked,
        app->techTreeRedQueenLocked, app->techTreeRedEnvoyLocked, app->techTreeRedLancerLocked, app->techTreeRedMageLocked,
        app->techTreeRedCatapultLocked, app->techTreeRedGoldLocked, app->techTreeRedPiecesLocked, app->techTreeRedPlatformLocked,
        app->techTreeBluePawnBuyable, app->techTreeBlueKnightBuyable, app->techTreeBlueBishopBuyable, app->techTreeBlueRookBuyable,
        app->techTreeBlueQueenBuyable, app->techTreeBlueEnvoyBuyable, app->techTreeBlueLancerBuyable, app->techTreeBlueMageBuyable,
        app->techTreeBlueCatapultBuyable, app->techTreeBlueGoldBuyable, app->techTreeBluePiecesBuyable, app->techTreeBluePlatformBuyable,
        app->techTreeRedPawnBuyable, app->techTreeRedKnightBuyable, app->techTreeRedBishopBuyable, app->techTreeRedRookBuyable,
        app->techTreeRedQueenBuyable, app->techTreeRedEnvoyBuyable, app->techTreeRedLancerBuyable, app->techTreeRedMageBuyable,
        app->techTreeRedCatapultBuyable, app->techTreeRedGoldBuyable, app->techTreeRedPiecesBuyable, app->techTreeRedPlatformBuyable,
        app->techTreeBluePawnBought, app->techTreeBlueKnightBought, app->techTreeBlueBishopBought, app->techTreeBlueRookBought,
        app->techTreeBlueQueenBought, app->techTreeBlueEnvoyBought, app->techTreeBlueLancerBought, app->techTreeBlueMageBought,
        app->techTreeBlueCatapultBought, app->techTreeBlueGoldBought, app->techTreeBluePiecesBought, app->techTreeBluePlatformBought,
        app->techTreeRedPawnBought, app->techTreeRedKnightBought, app->techTreeRedBishopBought, app->techTreeRedRookBought,
        app->techTreeRedQueenBought, app->techTreeRedEnvoyBought, app->techTreeRedLancerBought, app->techTreeRedMageBought,
        app->techTreeRedCatapultBought, app->techTreeRedGoldBought, app->techTreeRedPiecesBought, app->techTreeRedPlatformBought,
        app->UIRedOverlayAvailable, app->UIBlueOverlayAvailable, app->UIRedOverlayUnavailable, app->UIBlueOverlayUnavailable,
    };

    int count = sizeof(allTextures) / sizeof(SDL_Texture*);
    for (int i = 0; i < count; i++) {
        if (allTextures[i]) SDL_DestroyTexture(allTextures[i]);
    }
}