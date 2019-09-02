#include "TGEngine.hpp"
#include "gamecontent/Light.hpp"

using namespace std;
using namespace tge::tex;

void initEngine() {
	tge::nio::initFileSystem();
	properties = new prop::Properties();
	prop::readProperties("Properties.xml", properties);

	createWindowClass();
	TG_VECTOR_GET_SIZE_AND_RESIZE(window_list)
	createWindow(window_list[lastSize] = new Window);
	createInstance({
		#ifdef DEBUG 
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_KHRONOS_validation",
        "VK_LAYER_LUNARG_assistant_layer",
		"VK_LAYER_LUNARG_monitor",
		#endif
		    "VK_LAYER_VALVE_steam_overlay",
		    "VK_LAYER_NV_optimus",
			"VK_LAYER_AMD_switchable_graphics"
		}, {
		});
	createWindowSurfaces();
	createDevice({}, { "VK_LAYER_AMD_switchable_graphics" });
	prePipeline();
	initShader();
	initShaderPipes();

	tge::gmc::multiplier = (window_list[0]->height / (float)window_list[0]->width);

	createDepthTest();
	createColorResouce();
	createRenderpass();

	tge::gmc::initLight();
	tge::gmc::initCameras();
	initAllTextures();
	initDescriptors();

	allocateAllBuffers();
	fillUniformBuffer(&tge::gmc::camera_uniform, new glm::mat4(1.0f), sizeof(glm::mat4));

	for each (tge::gmc::LightActor * var in tge::gmc::lights) {
		var->updateLight();
	}
}

void startTGEngine() {

	createSwapchain();
	createFramebuffer();

	VertexBuffer main_buffer = {};
	main_buffer.max_vertex_count = 9000000;
	createVertexBuffer(&main_buffer);

	IndexBuffer index_buffer = {};
	index_buffer.size = 90000000;
	createIndexBuffer(&index_buffer);
	createCommandBuffer();

	tge::ui::ui_scene_entity.init();

	main_buffer.start();
	index_buffer.start();

	for(size_t i = 0; i < tge::gmc::models.size(); i++) {
		for (size_t j = 0; j < tge::gmc::models[i]->actors.size(); j++)
		{
			tge::gmc::models[i]->actors[j]->mesh->consume(&main_buffer, &index_buffer);
		}
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

	uint32_t counter = 0;

	while(true) {
		window_list[0]->pollevents();
		if(window_list[0]->close_request) {
			break;
		}
		if(window_list[0]->minimized) {
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

			for each (tge::gmc::LightActor * var in tge::gmc::lights) {
				var->updateLight();
			}

			startSingleTimeCommand();
			vlibBufferCopy.srcOffset = vlibBufferCopy.dstOffset = vertex_offset * VERTEX_SIZE;
			vlibBufferCopy.size = main_buffer.pointCount * VERTEX_SIZE;
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				main_buffer.stag_buf.staging_buffer,
				main_buffer.vertex_buffer,
				1,
				&vlibBufferCopy
			);
			vlibBufferCopy.srcOffset = 0;
			vlibBufferCopy.dstOffset = index_offset * sizeof(uint32_t);
			vlibBufferCopy.size = index_buffer.index_count * sizeof(uint32_t);
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				index_buffer.stag_buf.staging_buffer,
				index_buffer.index_buffer,
				1,
				&vlibBufferCopy
			);
			endSingleTimeCommand();
		}

		submit(&index_buffer, &main_buffer);
		present(&index_buffer, &main_buffer);

		// TESTING
		if (counter > 5) {
		//	exit(0);
		}
		counter++;
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
