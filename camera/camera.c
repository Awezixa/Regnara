#include "camera.h"

//clamp function = restriction of value to specific range
static float clampf(float value, float min_value, float max_value)
{
    if (value < min_value) return min_value;
    if (value > max_value) return max_value;
    return value;
}

void camera2d_init(Camera *camera, float x, float y, float zoom)
{
    if (!camera) return;
    camera->x = x;
    camera->y = y;
    camera->zoom = zoom;
}

void camera2d_add_zoom(Camera *camera, float delta, float min_zoom, float max_zoom)
{
    if (!camera) return;
    camera->zoom = clampf(camera->zoom + delta, min_zoom, max_zoom);//adjusts clamp value to meet player zoom of camera
}

void camera2d_follow_target(
    Camera *camera,
    float target_x,
    float target_y,
    float viewport_w,
    float viewport_h,
    float world_w,
    float world_h)
{
    if (!camera) return;
    if (camera->zoom <= 0.0001f) return;//safety feature to prevent division by 0 which can cause app to crash

    const float viewport_world_w = viewport_w / camera->zoom;//calcualtion how much world visible on screen based zoom level
    const float viewport_world_h = viewport_h / camera->zoom;

    camera->x = target_x - viewport_world_w * 0.5f;//camera/ screen calculation
    camera->y = target_y - viewport_world_h * 0.5f;//screen = wolrd position - camera position

    float max_cam_x = world_w - viewport_world_w;//same idea as above
    float max_cam_y = world_h - viewport_world_h;
    if (max_cam_x < 0.0f) max_cam_x = 0.0f;//prevent camera zoom being too small
    if (max_cam_y < 0.0f) max_cam_y = 0.0f;

    camera->x = clampf(camera->x, 0.0f, max_cam_x);
    camera->y = clampf(camera->y, 0.0f, max_cam_y);
}

SDL_FPoint camera2d_world_to_screen_point(const Camera *camera, float world_x, float world_y)
{
    SDL_FPoint point = {world_x, world_y};
    if (!camera) return point;//to prevent crashing returns original world coordinates

    point.x = (world_x - camera->x) * camera->zoom;//math behind making camera show a local space
    point.y = (world_y - camera->y) * camera->zoom;//brackets is local space offset and multiplication works with scaling
    return point;
}

SDL_FRect camera2d_world_to_screen_rect(
    const Camera *camera,
    float world_x,
    float world_y,
    float world_w,
    float world_h)
{
    SDL_FPoint top_left = camera2d_world_to_screen_point(camera, world_x, world_y);

    SDL_FRect rect = {
        top_left.x,
        top_left.y,
        world_w,
        world_h,
    };

    if (!camera) return rect;
    rect.w = world_w * camera->zoom;
    rect.h = world_h * camera->zoom;
    return rect;
}