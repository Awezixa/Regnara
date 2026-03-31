#include "transform.h"

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    Vec2 out = {a.x + b.x, a.y + b.y};
    //basic vector addition
    return out;
}

Vec2 vec2_sub(Vec2 a, Vec2 b)
{
    Vec2 out = {a.x - b.x, a.y - b.y};
    //basic vector subtraction
    return out;
}

Vec2 vec2_scale(Vec2 v, float s)
{
    Vec2 out = {v.x * s, v.y * s};
    //vector scaling.
    return out;
}

Vec2 vec2_normalized(Vec2 v)
{
    /*
    Scaling down a vector but maintaining direction
    */
    const float len_sq = v.x * v.x + v.y * v.y;
    if (len_sq <= 0.0001f) {
        Vec2 zero = {0.0f, 0.0f};//safety that returns vectors as 0 if too small
        return zero;
    }
    const float inv_len = 1.0f / SDL_sqrtf(len_sq);
    Vec2 out = {v.x * inv_len, v.y * inv_len};
    return out;
}

Vec2 vec2_rotate_deg(Vec2 v, float degrees)
{
    const float radians = degrees * (SDL_PI_F / 180.0f);
    const float c = SDL_cosf(radians);
    const float s = SDL_sinf(radians);
    Vec2 out = {v.x * c - v.y * s, v.x * s + v.y * c};
    return out;
}

Transform2D transform2d_make(float x, float y, float rotation_deg, float scale_x, float scale_y)
{
    /*
    specific order for multiple transformations:
    1. scale
    2. rotate
    3. translate
    */
    Transform2D out;
    out.position = (Vec2){x, y};
    out.rotationDeg = rotation_deg;
    out.scale = (Vec2){scale_x, scale_y};
    return out;
}

Vec2 transform2d_local_to_world(const Transform2D *parent, Vec2 local_point)
{
    if (!parent) return local_point;
    /*
    specific order for multiple transformations:
    1. scale
    2. rotate
    3. translate


    how to attach objects to other objects in game world based off parent obj in world
    eg. sword in player hand based on player itself
    */
    Vec2 scaled = {local_point.x * parent->scale.x, local_point.y * parent->scale.y};
    Vec2 rotated = vec2_rotate_deg(scaled, parent->rotationDeg);
    return vec2_add(parent->position, rotated);
}