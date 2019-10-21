#include "Pipe.hpp"
#include "window/Window.hpp"
#include "Descriptors.hpp"

namespace tge::pip {

	void createMaterialPipelines(VkPipeline* pipelines) {
		Window* win = windowList[0];
		vlibScissor.extent.height = (uint32_t)(vlibViewport.height = (float)win->height);
		vlibScissor.extent.width = (uint32_t)(vlibViewport.width = (float)win->width);

		vlibMultisampleState.rasterizationSamples = used_msaa_flag;

		vlibGraphicsPipelineCreateInfo.layout = pipelineLayout;
		vlibGraphicsPipelineCreateInfo.renderPass = render_pass;

		lastResult = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &vlibGraphicsPipelineCreateInfo, nullptr, &pipelines[lastSize]);
		CHECKFAIL;
	}
}