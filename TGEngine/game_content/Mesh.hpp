#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../game_content/Light.hpp"
#include "../pipeline/Shader.hpp"
#include "../pipeline/Pipe.hpp"

struct Material {

	Texture* texture;
	glm::vec4 color;

	uint32_t size; // count of vertices to draw for this matirial
	uint32_t offset; // the offset at wich this material starts (within the mesh)

	uint32_t pipeline_index;
	uint32_t descriptor_index;
	uint32_t layout_index;

	bool operator==(const Material& material);
	void createMaterialPipeline();
};

struct Mesh
{
	uint32_t first_index; // the first index for this mesh

	std::vector<Material> materials;
	std::vector<TGVertex> vertices;
	std::vector<uint32_t> indices;

	/*
     * this is used in the internal draw -> adds the mesh to the buffer
     */
	SINCE(0, 0, 4)
	void consume(VertexBuffer* vrt, IndexBuffer* ind);

	/*
	 * This adds a vertex to this mesh [same as add(TGVertex vrt)]
	 */
	SINCE(0, 0, 4)
	void operator<<(TGVertex vrt);

	/*
     * This adds a vertex to this mesh or the index of the vertex if it should already exist
     */
	SINCE(0, 0, 4)
    void add(TGVertex vrt);
};