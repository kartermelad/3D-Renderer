#include "mesh/mesh.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct { uint64_t key; int count; } EdgeCount;

static uint64_t pack_edge(uint32_t a, uint32_t b) {
    return ((uint64_t)a << 32) | b;
}

size_t mesh_get_boundary_edges(const Mesh* mesh, Edge** outEdges) {
    size_t triCount = mesh->indexCount / 3;
    size_t tableCap = triCount * 3;
    EdgeCount* table = calloc(tableCap, sizeof(*table));
    size_t used = 0;

    for (size_t t = 0; t < triCount; t++) {
        uint32_t vs[3] = {
            (uint32_t)mesh->indices[t*3 + 0],
            (uint32_t)mesh->indices[t*3 + 1],
            (uint32_t)mesh->indices[t*3 + 2]
        };
        for (int e = 0; e < 3; e++) {
            uint32_t a = vs[e], b = vs[(e+1) % 3];
            if (a > b) { uint32_t tmp = a; a = b; b = tmp; }
            uint64_t key = pack_edge(a, b);

            size_t idx = key % tableCap;
            while (table[idx].count > 0 && table[idx].key != key) {
                idx = (idx + 1) % tableCap;
            }

            if (table[idx].count == 0) {
                table[idx].key   = key;
                table[idx].count = 1;
                used++;
            } else {
                table[idx].count++;
            }
        }
    }

    Edge* edges = malloc(sizeof(*edges) * used);
    size_t outN = 0;
    for (size_t i = 0; i < tableCap; i++) {
        if (table[i].count == 1) {
            uint32_t a = (uint32_t)(table[i].key >> 32);
            uint32_t b = (uint32_t)table[i].key;
            edges[outN++] = (Edge){ a, b };
        }
    }

    free(table);
    *outEdges = edges;
    return outN;
}


Mesh* create_cube_mesh() {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) {
        return NULL;
    }

    mesh->vertexCount = 8;
    mesh->vertices = (Vertex*)malloc(mesh->vertexCount * sizeof(Vertex));
    if (!mesh->vertices) {
        free(mesh);
        return NULL;
    }

    mesh->vertices[0] = (Vertex){{-0.5f, -0.5f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 0, 0, 255}};
    mesh->vertices[1] = (Vertex){{0.5f, -0.5f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0, 255, 0, 255}};
    mesh->vertices[2] = (Vertex){{0.5f, 0.5f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0, 0, 255, 255}};
    mesh->vertices[3] = (Vertex){{-0.5f, 0.5f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 255, 0, 255}};
    mesh->vertices[4] = (Vertex){{-0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 0, 255, 255}};
    mesh->vertices[5] = (Vertex){{0.5f, -0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0, 255, 255, 255}};
    mesh->vertices[6] = (Vertex){{0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}};
    mesh->vertices[7] = (Vertex){{-0.5f, 0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {128, 128, 128, 255}};

    mesh->indexCount = 36;
    mesh->indices = (int*)malloc(mesh->indexCount * sizeof(int));
    if (!mesh->indices) {
        free(mesh->vertices);
        free(mesh);
        return NULL;
    }

    mesh->indices[0] = 0; mesh->indices[1] = 1; mesh->indices[2] = 2;
    mesh->indices[3] = 2; mesh->indices[4] = 3; mesh->indices[5] = 0;
    mesh->indices[6] = 5; mesh->indices[7] = 4; mesh->indices[8] = 7;
    mesh->indices[9] = 7; mesh->indices[10] = 6; mesh->indices[11] = 5;
    mesh->indices[12] = 4; mesh->indices[13] = 0; mesh->indices[14] = 3;
    mesh->indices[15] = 3; mesh->indices[16] = 7; mesh->indices[17] = 4;
    mesh->indices[18] = 1; mesh->indices[19] = 5; mesh->indices[20] = 6;
    mesh->indices[21] = 6; mesh->indices[22] = 2; mesh->indices[23] = 1;
    mesh->indices[24] = 3; mesh->indices[25] = 2; mesh->indices[26] = 6;
    mesh->indices[27] = 6; mesh->indices[28] = 7; mesh->indices[29] = 3;
    mesh->indices[30] = 4; mesh->indices[31] = 5; mesh->indices[32] = 1;
    mesh->indices[33] = 1; mesh->indices[34] = 0; mesh->indices[35] = 4;

    return mesh;
}

Mesh* create_pyramid_mesh() {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) {
        return NULL;
    }

    mesh->vertexCount = 5;
    mesh->vertices = (Vertex*)malloc(mesh->vertexCount * sizeof(Vertex));
    if (!mesh->vertices) {
        free(mesh);
        return NULL;
    }

    mesh->vertices[0] = (Vertex){{-0.5f, 0.0f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 0, 0, 255}};
    mesh->vertices[1] = (Vertex){{0.5f, 0.0f, -0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0, 255, 0, 255}};
    mesh->vertices[2] = (Vertex){{0.5f, 0.0f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0, 0, 255, 255}};
    mesh->vertices[3] = (Vertex){{-0.5f, 0.0f, 0.5f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 255, 0, 255}};
    mesh->vertices[4] = (Vertex){{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}};

    mesh->indexCount = 18;
    mesh->indices = (int*)malloc(mesh->indexCount * sizeof(int));
    if (!mesh->indices) {
        free(mesh->vertices);
        free(mesh);
        return NULL;
    }

    mesh->indices[0] = 4; mesh->indices[1] = 0; mesh->indices[2] = 1;
    mesh->indices[3] = 4; mesh->indices[4] = 1; mesh->indices[5] = 2;
    mesh->indices[6] = 4; mesh->indices[7] = 2; mesh->indices[8] = 3;
    mesh->indices[9] = 4; mesh->indices[10] = 3; mesh->indices[11] = 0;

    mesh->indices[12] = 0; mesh->indices[13] = 1; mesh->indices[14] = 2;
    mesh->indices[15] = 2; mesh->indices[16] = 3; mesh->indices[17] = 0;

    return mesh;
}

void destroy_mesh(Mesh* mesh) {
    if (!mesh) {
        return;
    }

    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}
