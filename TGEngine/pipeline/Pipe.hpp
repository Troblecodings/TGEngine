#pragma once

#include "../Stdbase.hpp"
#include "Device.hpp"
#include "RenderPass.hpp"
#include <vector>
#include "Descriptors.hpp"
#include "../vlib/VulkanPipeline.hpp"

namespace tge::pip {

	void createMaterialPipelines(Material* materials, uint32_t size, VkPipeline* pipelines);

}