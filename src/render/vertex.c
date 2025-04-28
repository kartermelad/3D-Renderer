#include "render/vertex.h"

Vec4 vertex_to_vec4(Vertex v) {
    return vec4_from_vec3(v.position, 1.0f);
}