#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../game_content/Light.hpp"

struct Material {

	Texture* texture;
	glm::vec4 color;
	uint32_t pipeline_index;

	bool operator==(const Material& material);
	void createMaterialPipeline();
};

struct Mesh
{
	Material material;
	uint64_t vertex_offset;
	uint64_t index_offset;
	std::vector<TGVertex> vertices;
	std::vector<uint32_t> indices;

	/*
     * Adds the mesh to the drawloop
     */
	SINCE(0, 0, 4)
	void consume(VertexBuffer* vrt, IndexBuffer* ind);

	SINCE(0, 0, 4)
	void operator<<(TGVertex vrt);

	SINCE(0, 0, 4)
    void add(TGVertex vrt);
};