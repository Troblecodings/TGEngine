#include "TGEngine.hpp"
#include "Pipeline.hpp"
#include "Draw.hpp"
#include "ObjLoader.hpp"
#include <cstddef>

nio::Properties properties;
uint32_t image_count = 3;

using namespace std;

void initTGEngine(App *app) {
	nio::readProperties("Properties.xml", &properties);
	createWindow(properties);
	createInstance(properties, { 
		"VK_LAYER_LUNARG_standard_validation", 
		"VK_LAYER_VALVE_steam_overlay", 
		"VK_LAYER_NV_optimus",
	}, { });
	createWindowSurface();
	createDevice({}, {});
	prePipeline();
	createRenderpass();
	createShader("vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	createShader("frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	createShaderInput(0, offsetof(Vertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32_SFLOAT);
	createPipeline();
	createSwapchain();
	createFramebuffer();
    
	vector<Vertex> vertieces = {};

	createVertexBuffer(50000);
	fillVertexBuffer(vertieces);
	createCommandBuffer();
	fillCommandBuffer();
	createSemaphores();

	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			break;
		}
		app->drawloop(&vertieces);
		draw(vertieces);
	}

	destroySemaphores();
	destroyCommandBuffer();
	destroyVertexBuffer();
	destroyFrameBuffer();
	destroySwapchain();
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDevice();
	destroyWindow();
	destroyInstance();
}
