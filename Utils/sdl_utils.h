#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

// =======================================================
// SDL SYSTEMS
// =======================================================
SDL_Window *sdl_initialize_window(const char *app_name, int app_width, int app_height);
SDL_Renderer *sdl_initialize_renderer(SDL_Window *window);

// =======================================================
// SOUND
// =======================================================
typedef struct Sound
{
    Uint8 *wav_data;
    Uint32 wav_data_len;
    SDL_AudioStream *stream;
} Sound;

int sdl_initialize_audio();
bool init_sound(const char *fname, Sound *sound);
void playSound(Sound *sound);
void stopSound(Sound *sound);

// =======================================================
// TEXTURE
// =======================================================

// Load a texture from a PNG/JPG/etc in the same folder as the executable.
// Returns NULL on error.
SDL_Texture *sdl_load_texture(SDL_Renderer *renderer, const char *file_path);

// =======================================================
// TEXT RENDERING
// =======================================================
void showText(SDL_Renderer *renderer, int x, int y, const char *text, SDL_Color color);

#endif // SDL_UTILS_H#endif // SDL_UTILS_H
