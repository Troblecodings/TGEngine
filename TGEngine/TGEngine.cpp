#include "TGEngine.hpp"

using namespace std;

void initTGEngine(Window* window, void (*draw)(IndexBuffer*, VertexBuffer*), void (*init)(void)) {
	nio::queryCWD();
	properties = new prop::Properties();
	prop::readProperties("Properties.xml", properties);

	Texture no_texture = {};
	no_texture.width = 1;
	no_texture.height = 1;
	no_texture.image_data = new stbi_uc[4];
	no_texture.image_data[0] = 0;
	no_texture.image_data[1] = 0;
	no_texture.image_data[2] = 0;
	no_texture.image_data[3] = 0;
	createTexture(&no_texture);

	createWindowClass();
	createWindow(window);
	createInstance({
		#ifdef DEBUG 
		"VK_LAYER_LUNARG_standard_validation",
		#endif
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus"
		}, { });

	createWindowSurfaces();
	createDevice({}, {});
	prePipeline();

	init();
	createDepthTest();
	createRenderpass();
	createShader();
	createShaderInput(0, offsetof(TGVertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(TGVertex, color), VK_FORMAT_R32G32B32A32_SFLOAT);
	createShaderInput(2, offsetof(TGVertex, uv), VK_FORMAT_R32G32_SFLOAT);
	createShaderInput(3, offsetof(TGVertex, color_only), VK_FORMAT_R32_UINT);
	initAllTextures();
	initCameras();

	createPipelineLayout();
	createPipeline();
	createSwapchain();
	createFramebuffer();

	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 500000;
	createVertexBuffer(&main_buffer);

	IndexBuffer index_buffer = {};
	index_buffer.size = 500000;
	createIndexBuffer(&index_buffer);

	allocateAllBuffers();
	createAllDescriptorSets();
	fillUniformBuffer(&camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));

	createCommandBuffer();
	singleTimeCommand();
	createSemaphores();

	addTextures();

	while (true) {
		window->pollevents();
		if (window->close_request) {
			break;
		}
		if (window->minimized) {
			continue;
		}
		startdraw();
		main_buffer.start();
		index_buffer.start();
		draw(&index_buffer, &main_buffer);
		main_buffer.end();
		index_buffer.end();
		last_result = vkDeviceWaitIdle(device);
		HANDEL(last_result)
		fillCommandBuffer(&index_buffer, &main_buffer, image_index);
		submit();
		present();
	}

	destroyAllTextures();
	destroySemaphores();
	destroyCommandBuffer();
	destroyDescriptors();
	destroyMemory();
	destroyIndexBuffer(&index_buffer);
	destroyFrameBuffer();
	destroySwapchain();
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDepthTest();
	destroyDevice();
	destroyWindows();
	destroyInstance();
}
