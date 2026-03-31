#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <SDL3/SDL.h>

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef struct Transform2D {
    Vec2 position; /* Translation in world space */
    float rotationDeg;
    Vec2 scale;
} Transform2D;

Vec2 vec2_add(Vec2 a, Vec2 b);
Vec2 vec2_sub(Vec2 a, Vec2 b);
Vec2 vec2_scale(Vec2 v, float s);
Vec2 vec2_normalized(Vec2 v);
/**
* Description: Return normalized (length 1) direction.
* Input: vector v.
* Output: normalized vector; returns (0,0) if length is near zero.
*/

Vec2 vec2_rotate_deg(Vec2 v, float degrees);
Transform2D transform2d_make(float x, float y, float rotation_deg, float scale_x, float scale_y);
/**
* Description: Build one Transform2D.
* Input: position, rotation, scale values.
* Output: initialized Transform2D.
*/

Vec2 transform2d_local_to_world(const Transform2D *parent, Vec2 local_point);
/**
* Description: Convert local point to world point using parent transform.
* Input: parent transform, local point.
* Output: world-space point.
*/
#endif
