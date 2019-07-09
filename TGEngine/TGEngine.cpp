#include "TGEngine.hpp"

using namespace std;
using namespace tge::tex;

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

	tge::gmc::initCameras();
	initAllTextures();

	initDescriptors();
	initShader();
	initShaderPipes();

	allocateAllBuffers();
	fillUniformBuffer(&tge::gmc::camera_uniform, &glm::mat4(1.0f), sizeof(glm::mat4));

	createSwapchain();
	createFramebuffer();

	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 9000000;
	createVertexBuffer(&main_buffer);

	IndexBuffer index_buffer = {};
	index_buffer.size = 9000000;
	createIndexBuffer(&index_buffer);
	createCommandBuffer();
	tge::gmc::multiplier = (window->height / (float)window->width);

	tge::ui::ui_scene_entity.init();

	main_buffer.start();
	index_buffer.start();

	for(size_t i = 0; i < tge::gmc::actors.size(); i++) {
		tge::gmc::actors[i]->mesh->consume(&main_buffer, &index_buffer);
	}
	OUT_LV_DEBUG(tge::gmc::materiallist.size())
		for each(tge::gmc::Material * mat in tge::gmc::materiallist) {
			OUT_LV_DEBUG(mat)
				OUT_LV_DEBUG(mat->getType())
				mat->createMaterial();
		}

	//draw(&index_buffer, &main_buffer);

	index_offset = index_buffer.index_count;
	vertex_offset = main_buffer.pointCount;

	tge::ui::ui_scene_entity.draw(&index_buffer, &main_buffer);

	main_buffer.end();
	index_buffer.end();

	fillCommandBuffer(&index_buffer, &main_buffer);

	startupCommands();
	createSemaphores();

	clock_t last_time = clock();

	while(true) {
		window->pollevents();
		if(window->close_request) {
			break;
		}
		if(window->minimized) {
			continue;
		}
		startdraw(&index_buffer, &main_buffer);

		clock_t current_time = clock();
		clock_t delta = current_time - last_time;

		if(delta >= (CLOCKS_PER_SEC / 60)) {
			last_time = current_time;

			tge::ui::ui_scene_entity.update();
			main_buffer.pointCount = vertex_offset;
			main_buffer.start();
			index_buffer.start();
			tge::ui::ui_scene_entity.draw(&index_buffer, &main_buffer);
			main_buffer.end();
			index_buffer.end();

			startSingleTimeCommand();
			vlib_buffer_copy.srcOffset = vlib_buffer_copy.dstOffset = vertex_offset * VERTEX_SIZE;
			vlib_buffer_copy.size = main_buffer.pointCount * VERTEX_SIZE;
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
