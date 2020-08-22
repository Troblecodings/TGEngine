#pragma once

#include "../../public/Stdbase.hpp"
#include "../../public/resources/ShaderPipes.hpp"

namespace tge::pip {

	typedef VkCullModeFlags CullMode;
	typedef VkPolygonMode PolygonMode;
	typedef VkVertexInputBindingDescription VertexInputBinding;

	constexpr uint32_t PIPELINE_COUNT = 2;

	struct PipelineInputInfo {
		ShaderPipe          pipe;
		CullMode            cullMode;
		PolygonMode         polygonMode;
		VertexInputBinding* inputBindings;
		uint32_t            inputBindingCount;
	};

	extern VkPipeline defaultPipeline[PIPELINE_COUNT];

	void createPipelines(PipelineInputInfo* input, uint32_t size, VkPipeline* pipelines);

	void initPipelines();

	void destroyPipelines();
}