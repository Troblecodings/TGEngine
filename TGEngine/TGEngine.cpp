#include "TGEngine.hpp"
#include "Pipeline/Pipeline.hpp"
#include "Pipeline\Draw.hpp"
#include "IO/ObjLoader.hpp"
#include <cstddef>

using namespace std;

void initTGEngine(App *app) {
	nio::readProperties("Properties.xml", &properties);
	createWindow(properties);
	createInstance(properties, { 
        #ifdef DEBUG 
		"VK_LAYER_LUNARG_standard_validation",
        #endif
		"VK_LAYER_VALVE_steam_overlay", 
		"VK_LAYER_NV_optimus"
	}, { });
	createWindowSurface();
	createDevice({}, {});
	prePipeline();
	createRenderpass();
	createShader("vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	createShader("frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	createShaderInput(0, offsetof(Vertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32A32_SFLOAT);

	addDescriptor(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT);

	createPipeline();
	createSwapchain();
	createFramebuffer();
    
	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 500000;

	createVertexBuffer(&main_buffer);

	uint32_t uniform_scale_buffer = createUniformBuffer(sizeof(glm::vec2));

	allocateAllBuffers();

	vector<glm::vec2> scale = { { 1, 1 } };
	if (height > width) {
		scale[0] = {
			1,
			(float)((float)width / (float)height)
		};
	}
	else if(height < width) {
		scale[0] = {
			(float)((float)height / (float)width),
			1
		};
	}
	fillUniformBuffer(uniform_scale_buffer, scale.data(), sizeof(glm::vec2));

	createDescriptorsForUniformBuffers({uniform_scale_buffer});
	updateDescriptorSet(uniform_scale_buffer, 0, sizeof(glm::vec2));

	createCommandBuffer();
	createSemaphores();


	while (true) {
		window.pollevents();
		if (window.close_request) {
			break;
		}
		main_buffer.start();
		app->drawloop(&main_buffer);
		main_buffer.end();
		vkDeviceWaitIdle(device);
		fillCommandBuffer(&main_buffer);
		draw(&main_buffer);
	}

	destroySemaphores();
	destroyCommandBuffer();
	destroyDescriptors();
	destroyMemory();
	destroyFrameBuffer();
	destroySwapchain();
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDevice();
	destroyWindow();
	destroyInstance();
}
