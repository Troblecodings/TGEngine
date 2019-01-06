#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../model/FBX_Loader.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../util/VectorUtil.hpp"

struct Mesh
{
	std::vector<TGVertex> vertices;
	std::vector<uint32_t> indices;

	/*
     * Adds the mesh to the drawloop
     */
	SINCE(0, 0, 4)
	void consume(VertexBuffer* vrt, IndexBuffer* ind);
};

/*
 * Preproccess data for vertex buffer
 * This is triangulated via a fan-triangulation algorithem
 *  -> Inputs should only be models that have convex pylogens without holes
 */
SINCE(0, 0, 4)
void loadFromFBX(Mesh* msh, char* path);

/*
 * Adds a vertex to the given mesh
 *  -> if the vertex should already be in the mesh it is not added instead an other index is added
 *    -> returns index of vertex
 */
SINCE(0, 0, 4)
uint32_t addVertex(Mesh* mesh, TGVertex vert);