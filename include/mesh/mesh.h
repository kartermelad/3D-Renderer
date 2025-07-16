#ifndef MESH_H
#define MESH_H

#include "render/vertex.h"

// Definition of the Mesh struct
typedef struct {
    Vertex* vertices;
    int* indices;
    size_t vertexCount;
    size_t indexCount;
} Mesh;

// Undirected edge between two vertex indices
typedef struct {
    uint32_t a, b;
} Edge;

/**
 * Creates a cube mesh with predefined vertices and indices
 * 
 * @return Pointer to a dynamically allocated Mesh representing a cube
 */
Mesh* create_cube_mesh();

/**
 * Creates a pyramid mesh with predefined vertices and indices
 * 
 * @return Pointer to a dynamically allocated Mesh representing a pyramid
 */
Mesh* create_pyramid_mesh();

/**
 * Frees the memory allocated for a mesh
 * 
 * @param mesh Pointer to the Mesh to be destroyed
 */
void destroy_mesh(Mesh* mesh);

/**
 * Loads a mesh from an OBJ file
 * 
 * @return Pointer to a dynamically allocated Mesh loaded from the OBJ file
 */
// Mesh* load_mesh_from_obj(const char* filepath);

/**
 * Finds all boundary edges (edges belonging to only one triangle) in the mesh.
 *
 * @param mesh Pointer to the mesh to analyze
 * @param outEdges Output pointer to an array of Edge structs (allocated inside the function, must be freed by the caller)
 * @return The number of boundary edges found
 */
size_t mesh_get_boundary_edges(const Mesh* mesh, Edge** outEdges);

#endif