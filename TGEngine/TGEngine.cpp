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
	createShader("vertexshader.spv", VK_SHADER_STAGE_VERTEX_BIT);
	createShader("fragmentshader.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	createShaderInput(0, offsetof(Vertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32A32_SFLOAT);
	createShaderInput(2, offsetof(Vertex, uv), VK_FORMAT_R32G32_SFLOAT);

	UniformBuffer uniform_scale_buffer = {
		sizeof(glm::vec2),
	    { 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT }
	};
	createUniformBuffer(&uniform_scale_buffer);

	Camera cam = {
		{
			0,
			0,
			0.2F,
			0,
			0,
			0,
        }
	};
	createCamera(&cam);
	initAllTextures();

	createPipelineLayout();
	createPipeline();
	createSwapchain();
	createFramebuffer();
    
	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 500000;
	createVertexBuffer(&main_buffer);

	allocateAllBuffers();
	createAllDescriptorSets();

	if (height > width) {		
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2(1, (float)((float)width / (float)height)), sizeof(glm::vec2));
	}
	else if (height < width) {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2((float)((float)height / (float)width), 1), sizeof(glm::vec2));
	}
	else {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2(1, 1), sizeof(glm::vec2));
	}

	cam.updateCamera();

	createCommandBuffer();
	singleTimeCommand();
	updateShader();
	createSemaphores();

	uint64_t time = 0;

	while (true) {
		window.pollevents();
		if (window.close_request) {
			break;
		}
		uint64_t c_time = std::clock();
		if (time + 16 <= c_time) {
			time = c_time;
			main_buffer.start();
			uint32_t old_size = main_buffer.count_of_points;
			app->drawloop(&main_buffer);
			main_buffer.end();
			if (old_size != main_buffer.count_of_points) {
				vkDeviceWaitIdle(device);
				fillCommandBuffer(&main_buffer);
			}
		}
		draw(&main_buffer);
	}

	destroyAllTextures();
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
