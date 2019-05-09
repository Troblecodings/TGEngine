#pragma once

#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../pipeline/Pipe.hpp"

/*
 * Material can hold a texture and a color
 * -> Can not be changed in vulkan runtime
 * -> needs pipeline recreation
 */
SINCE(0, 0, 4)
class Material {

public:
	Texture* texture;
	glm::vec4 color;

	bool isUI = false;

	uint32_t pipeline_index;
	uint32_t descriptor_index;
	uint32_t layout_index;

	virtual void createMaterial();
	virtual void createUIMaterial();

	void destroy();

	bool operator==(const Material& material);
};

/*
 * Defines which part of the buffer has which material
 */
SINCE(0, 0, 4)
struct RenderOffsets {

	uint32_t material; // index in the @materials array of the material to use

	uint32_t size; // count of vertices to draw for this matirial
	uint32_t offset; // the offset at wich this material starts (global)
};

extern std::vector<Material> materials;
extern std::vector<RenderOffsets> render_offset;
