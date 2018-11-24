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

	void consume(VertexBuffer* vrt, IndexBuffer* ind);
};

void loadFromFBX(Mesh* msh, char* path);