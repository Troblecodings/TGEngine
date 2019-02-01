#include "TGEngine.hpp"

using namespace std;

void initTGEngine(Window* window, void (*draw)(IndexBuffer*, VertexBuffer*), void (*init)(void)) {
	nio::initFileSystem();
	properties = new prop::Properties();
	prop::readProperties("Properties.xml", properties);

	createWindowClass();
	createWindow(window);
	createInstance({
		#ifdef DEBUG 
		"VK_LAYER_LUNARG_standard_validation",
		#endif
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus"
		}, { 
#ifdef DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
		});

	createWindowSurfaces();
	createDevice({}, {});
	prePipeline();

	Texture no_texture = {};
	no_texture.width = 1;
	no_texture.height = 1;
	no_texture.image_data = new stbi_uc[4];
	no_texture.image_data[0] = 1;
	no_texture.image_data[1] = 0;
	no_texture.image_data[2] = 1;
	no_texture.image_data[3] = 1;
	createTexture(&no_texture);

	init();
	createDepthTest();
	createColorResouce();
	createRenderpass();
	createShader();
	createShaderInput(0, offsetof(TGVertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(TGVertex, color), VK_FORMAT_R32G32B32A32_SFLOAT);
	createShaderInput(2, offsetof(TGVertex, uv), VK_FORMAT_R32G32_SFLOAT);
	createShaderInput(3, offsetof(TGVertex, color_only), VK_FORMAT_R32_UINT);
	createShaderInput(4, offsetof(TGVertex, normal), VK_FORMAT_R32G32B32_SFLOAT);
	initAllTextures();
	initCameras();
	initDescriptors();

	createDesctiptorLayout();
	createPipeline();
	createSwapchain();
	createFramebuffer();

	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 900000;
	createVertexBuffer(&main_buffer);

	IndexBuffer index_buffer = {};
	index_buffer.size = 9900000;
	createIndexBuffer(&index_buffer);

	allocateAllBuffers();
	createDescriptorSet();
	fillUniformBuffer(&camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));

	createCommandBuffer();
	main_buffer.start();
	index_buffer.start();
	for (Actor act : actors) {
		act.mesh->consume(&main_buffer, &index_buffer);
	}
	draw(&index_buffer, &main_buffer);
	main_buffer.end();
	index_buffer.end();

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
	destroyVertexBuffer(&main_buffer);
	destroyStagingBuffer();
	destroyFrameBuffer();
	destroySwapchain();
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDepthTest();
	destroyColorResouce();
	destroyDevice();
	destroyWindows();
	destroyInstance();
}
