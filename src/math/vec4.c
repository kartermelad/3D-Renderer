#include "math/vec4.h"

Vec4 vec4_from_vec3(Vec3 v, float w) {
    Vec4 result = {v.x, v.y, v.z, w};
    return result;
}