#include "TGEngine.hpp"

using namespace std;

void initTGEngine(Window* window, void(*draw)(IndexBuffer*, VertexBuffer*), void(*init)(void)) {
	tge::nio::initFileSystem();
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
		});
	createWindowSurfaces();
	createDevice({}, {});
	prePipeline();

	uint8_t imageData[4] = { 1, 1, 1, 1 };
	Texture defaultTexture = Texture(imageData, 1, 1);

	init();

	createDepthTest();
	createColorResouce();
	createRenderpass();
	initShader();
	createShaderInput(0, offsetof(TGVertex, position), VK_FORMAT_R32G32B32_SFLOAT);
	createShaderInput(1, offsetof(TGVertex, uv), VK_FORMAT_R32G32_SFLOAT);
	createShaderInput(2, offsetof(TGVertex, normal), VK_FORMAT_R32G32B32_SFLOAT);

	addDescriptorBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	addDescriptorBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	addDescriptorBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);

	initCameras();
	initLight();

	allocateAllBuffers();
	fillUniformBuffer(&camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));

	initAllTextures();
	initDescriptors();

	createSwapchain();
	createFramebuffer();

	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 9000000;
	createVertexBuffer(&main_buffer);

	IndexBuffer index_buffer = {};
	index_buffer.size = 9000000;
	createIndexBuffer(&index_buffer);
	createCommandBuffer();
	multiplier = (window->height / (float)window->width);
	fillUniformBuffer(&ui_camera_uniform, &glm::mat4(1), sizeof(glm::mat4));

	tge::ui::ui_scene_entity.init();

	main_buffer.start();
	index_buffer.start();

	for (size_t i = 0; i < actors.size(); i++)
	{
		actors[i].mesh->consume(&main_buffer, &index_buffer);
	}
	for (size_t i = 0; i < materials.size(); i++)
	{
		if (materials[i].isUI) {
			materials[i].createUIMaterial();
		}
		else {
			materials[i].createMaterial();
		}
	}

	//draw(&index_buffer, &main_buffer);

	index_offset = index_buffer.index_count;
	vertex_offset = main_buffer.count_of_points;

	tge::ui::ui_scene_entity.draw(&index_buffer, &main_buffer);

	main_buffer.end();
	index_buffer.end();

	setLightPosition({ 0, 0, -10 });

	fillCommandBuffer(&index_buffer, &main_buffer);

	startupCommands();
	createSemaphores();

	clock_t last_time = clock();

	while (true) {
		window->pollevents();
		if (window->close_request) {
			break;
		}
		if (window->minimized) {
			continue;
		}
		startdraw(&index_buffer, &main_buffer);

		clock_t current_time = clock();
		clock_t delta = current_time - last_time;

		if (delta >= (CLOCKS_PER_SEC / 60)) {
			last_time = current_time;

			tge::ui::ui_scene_entity.update();
			main_buffer.count_of_points = vertex_offset;
			main_buffer.start();
			index_buffer.start();
			tge::ui::ui_scene_entity.draw(&index_buffer, &main_buffer);
			main_buffer.end();
			index_buffer.end();

			startSingleTimeCommand();
			vlib_buffer_copy.srcOffset = vlib_buffer_copy.dstOffset = vertex_offset * VERTEX_SIZE;
			vlib_buffer_copy.size = main_buffer.count_of_points * VERTEX_SIZE;
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				main_buffer.stag_buf.staging_buffer,
				main_buffer.vertex_buffer,
				1,
				&vlib_buffer_copy
			);
			vlib_buffer_copy.srcOffset = 0;
			vlib_buffer_copy.dstOffset = index_offset * sizeof(uint32_t);
			vlib_buffer_copy.size = index_buffer.index_count * sizeof(uint32_t);
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				index_buffer.stag_buf.staging_buffer,
				index_buffer.index_buffer,
				1,
				&vlib_buffer_copy
			);
			endSingleTimeCommand();
		}

		submit(&index_buffer, &main_buffer);
		present(&index_buffer, &main_buffer);
	}

	destroyAllTextures();
	destroySemaphores();
	destroyCommandBuffer();
	destroyMemory();
	destroyIndexBuffer(&index_buffer);
	destroyVertexBuffer(&main_buffer);
	destroyStagingBuffer();
	destroyFrameBuffer();
	destroySwapchain();
	destroyDescriptors();
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
	destroyPipeline();
	destroyShaders();
	destroyRenderPass();
	destroyDepthTest();
	destroyColorResouce();
	destroyDevice();
	destroyWindows();
	destroyInstance();
}
