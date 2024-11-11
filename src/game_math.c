#pragma once

#include "root.h"
#include "game_math.h"

bool floatAlmostEquals(f32 f1, f32 f2, f32 eps) {
    return (fabs(f1 - f2) < eps);
}

vec2 vec2Add(vec2 v1, vec2 v2) {
    return Vector2Add(v1, v2);
}

vec2 vec2Sub(vec2 v1, vec2 v2) {
    return Vector2Subtract(v1, v2);
}

vec2 vec2Normalize(vec2 v) {
    return Vector2Normalize(v);
}

vec2 vec2Scale(vec2 v, f32 scale) {
    return Vector2Scale(v, scale);
}

vec2 vec2SubtractValue(vec2 v, f32 sub) {
    return Vector2SubtractValue(v, sub);
}

f32 vec2Length(vec2 v) {
    return Vector2Length(v);
}

f32 vec2Distance(vec2 v1, vec2 v2) {
    return Vector2Distance(v1, v2);
}

bool vec2AlmostEquals(vec2 v1, vec2 v2) {
    return Vector2Equals(v1, v2);
    // f32 eps = 1e-6;
    // return floatAlmostEquals(v1.x, v2.x, eps) && floatAlmostEquals(v1.y, v2.y, eps); 
}
