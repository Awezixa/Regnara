#ifndef CAMERA_H
#define CAMERA_H
#include "transform.h"

typedef struct AppState AppState;

typedef struct Camera{
    float x;
    float y;
    float zoom;
}Camera;

//Mostly borrowed from Cesars code
/* Initialize camera state. */
/**
* Description: Initialize camera values.
* Input: camera pointer, x, y, zoom.
* Output: none.
*/
void camera2d_init(Camera *camera, float x, float y, float zoom);

/* Add zoom delta and clamp to [min_zoom, max_zoom]. */
/**
* Description: Add zoom delta and clamp into [min_zoom, max_zoom].
* Input: camera pointer, delta, min_zoom, max_zoom.
* Output: none.
*/

void camera2d_zoom_to_mouse(Camera *camera, float delta, float min_zoom, float max_zoom, float mouse_x, float mouse_y);
/*
* Follow a world-space target and clamp camera to world bounds.
* viewport_w / viewport_h are in screen pixels.
* world_w / world_h are in world units.
* Description: Move camera to follow target and clamp inside world
    bounds.
* Input: camera, target position, viewport size, world size.
* Output: none.
*/

void camera2d_follow_target(
    Camera *camera,
    float target_x,
    float target_y,
    float viewport_w,
    float viewport_h,
    float world_w,
    float world_h
);

/* Convert one world-space point to screen-space point. */
/**
* Description: Convert world point to screen point.
* Input: camera, world_x, world_y.
* Output: SDL_FPoint in screen space.
*/
SDL_FPoint camera2d_world_to_screen_point(const Camera *camera, float world_x, float world_y);

/* Convert one world-space rect to screen-space rect.
* Description: Convert world rect to screen rect.
* Input: camera, world rect.
* Output: SDL_FRect in screen space.
*/
SDL_FRect camera2d_world_to_screen_rect(
    const Camera *camera,
    float world_x,
    float world_y,
    float world_w,
    float world_h);
    //makes camera show local space
#endif