#include "TGEngine.hpp"

using namespace std;

void initTGEngine(Window* window, void(*draw)(IndexBuffer*, VertexBuffer*), void(*init)(void)) {
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
	initLight();
	initDescriptors();

	createDesctiptorLayout();
	createDesctiptorLayout();
	vlib_rasterization_state.cullMode = VK_CULL_MODE_FRONT_BIT;
	createPipeline();
	vlib_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
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
	createDescriptorSet();
	camera_uniform.descriptor.descriptor_set = 0;
	fillUniformBuffer(&camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));
	ui_camera_uniform.descriptor.descriptor_set = 1;
	ui_camera_uniform.descriptor.binding = 1;
	updateDescriptorSet(&camera_uniform.descriptor, sizeof(glm::mat4));

	multiplier = (window->width / (float)window->height);
	multiplierx = (window_list[0]->height / (float)window_list[0]->width);
	fillUniformBuffer(&ui_camera_uniform, &glm::ortho(-multiplier, multiplier, -1.0f, 1.0f), sizeof(glm::mat4));
	updateDescriptorSet(&ui_camera_uniform.descriptor, sizeof(glm::mat4));

	setLightPosition({ 1, 1, 1 });
	light_buffer.descriptor.descriptor_set = 0;
	light_buffer.descriptor.binding = 2;
	updateDescriptorSet(&light_buffer.descriptor, sizeof(glm::vec3));
	light_buffer.descriptor.descriptor_set = 1;
	updateDescriptorSet(&light_buffer.descriptor, sizeof(glm::vec3));

	createCommandBuffer();

	main_buffer.start();
	index_buffer.start();
	for (Actor act : actors) {
		act.mesh->consume(&main_buffer, &index_buffer);
	}
	draw(&index_buffer, &main_buffer);
	index_offset = index_buffer.index_count;
	vertex_offset = main_buffer.count_of_points;
	tg_ui::ui_scene_entity.draw(&index_buffer, &main_buffer);
	main_buffer.end();
	index_buffer.end();
	fillCommandBuffer(&index_buffer, &main_buffer);

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
		startdraw(&index_buffer, &main_buffer);
		submit(&index_buffer, &main_buffer);
		present(&index_buffer, &main_buffer);
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
