#include "math/vec2.h"

float vec2_cross(Vec2 a, Vec2 b) {
    float result = (a.x * b.y) - (a.y * b.x);
    return result;
}

Vec2 vec2_subtract(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}