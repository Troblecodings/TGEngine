#include "TGEngine.hpp"
#include "Pipeline.hpp"
#include "Draw.hpp"
#include "ObjLoader.hpp"
#include <cstddef>

nio::Properties properties;
uint32_t image_count = 3;

using namespace std;
#ifdef USE_INDEX_BUFFER
vector<uint32_t> indicies = {};
#endif

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
	createShaderInput(1, offsetof(Vertex, color), VK_FORMAT_R32G32B32_SFLOAT);
	createPipeline();
	createSwapchain();
	createFramebuffer();
    
	vector<Vertex> vertieces = {};
	createVertexBuffer(50000);

    #ifdef USE_INDEX_BUFFER
	createIndexBuffer(50000);
    #endif 

	allocateAllBuffers();

	createCommandBuffer();
	fillCommandBuffer();
	createSemaphores();

	while (true) {
		window.pollevents();
		if (window.close_request) {
			break;
		}
		app->drawloop(&vertieces);
		draw(&vertieces);
	}

	destroySemaphores();
	destroyCommandBuffer();
	destroyMemory();
	destroyIndexBuffer();
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
