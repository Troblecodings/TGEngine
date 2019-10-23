#pragma once

#include "../Stdbase.hpp"
#include "../resources/ShaderPipes.hpp"

namespace tge::pip {

	struct PipelineInputInfo {
		ShaderPipe pipe;
		uint32_t stride;
		uint32_t width;
		uint32_t height;
		VkCullModeFlags cullMode;
		VkPolygonMode polygonMode;
		float lineWidth;
	};

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines);

}