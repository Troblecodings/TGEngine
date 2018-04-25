#include "TGEngine.hpp"
#include "Pipeline/Pipeline.hpp"
#include "Pipeline\Draw.hpp"
#include "IO/ObjLoader.hpp"
#include <cstddef>

using namespace std;

void initTGEngine(App *app) {
	nio::readProperties("Properties.xml", &properties);
	createWindowClass();
	createWindow(&app->main_window, &properties);
	createInstance(&properties, { 
        #ifdef DEBUG 
		"VK_LAYER_LUNARG_standard_validation",
        #endif
		"VK_LAYER_VALVE_steam_overlay", 
		"VK_LAYER_NV_optimus"
	}, { });

	app->main_window.createWindowSurface();
	createDevice({}, {});
	prePipeline();
	createRenderpass();
	createShader();
	createShaderInput(0, offsetof(Vertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32A32_SFLOAT);
	createShaderInput(2, offsetof(Vertex, uv), VK_FORMAT_R32G32_SFLOAT);
	createShaderInput(3, offsetof(Vertex, only_color), VK_FORMAT_R32_UINT);

	UniformBuffer uniform_scale_buffer = {
		sizeof(glm::vec2),
	    { VK_SHADER_STAGE_VERTEX_BIT }
	};
	createUniformBuffer(&uniform_scale_buffer);
	initAllTextures();

	Camera cam;
	cam.speed = 0.001f;
	createCamera(&cam);

	createPipelineLayout();
	createPipeline();
	createSwapchain();
	createFramebuffer();
    
	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 500000;
	createVertexBuffer(&main_buffer);

	allocateAllBuffers();
	createAllDescriptorSets();

	if (app->main_window.height > app->main_window.width) {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2(1, (float)((float)app->main_window.width / (float)app->main_window.height)), sizeof(glm::vec2));
	}
	else if (app->main_window.height < app->main_window.width) {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2((float)((float)app->main_window.height / (float)app->main_window.width), 1), sizeof(glm::vec2));
	}
	else {
		fillUniformBuffer(&uniform_scale_buffer, (uint8_t*) &glm::vec2(1, 1), sizeof(glm::vec2));
	}
	cam.updateCamera();

	createCommandBuffer();
	singleTimeCommand();
	createSemaphores();

	uint64_t time = 0;

	while (true) {
		app->main_window.pollevents();
		if (app->main_window.close_request) {
			break;
		}
		uint64_t c_time = std::clock();
		if (time + 16 <= c_time) {
			time = c_time;
			main_buffer.start();
			uint32_t old_size = main_buffer.count_of_points;
			app->drawloop(&main_buffer);
			main_buffer.end();
		}
		vkDeviceWaitIdle(device);
		fillCommandBuffer(&main_buffer);
		startdraw();
		draw();
		present();
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
	app->main_window.destroy();
	destroyInstance();
}
