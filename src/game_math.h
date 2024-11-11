#pragma once

#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "root.h"

// TODO: Decide if I wanna try to implement some of these things myself, for now, these will call raymath functions

/*
typedef struct {
    f32 x;
    f32 y;
} vec2;
*/

bool floatAlmostEquals(f32 f1, f32 f2, f32 eps);

vec2 vec2Add(vec2 v1, vec2 v2);
vec2 vec2Sub(vec2 v1, vec2 v2);
vec2 vec2Scale(vec2 v, f32 scale);
vec2 vec2Normalize(vec2 v);
vec2 vec2SubtractValue(vec2 v, f32 sub);

f32 vec2Length(vec2 v);
f32 vec2Distance(vec2 v1, vec2 v2);

bool vec2AlmostEquals(vec2 v1, vec2 v2);

#endif
