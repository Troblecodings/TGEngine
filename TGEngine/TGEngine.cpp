#include "TGEngine.hpp"
#include "gamecontent/Light.hpp"

using namespace std;
using namespace tge::tex;

void initEngine() {
	tge::nio::initFileSystem();
	properties = new prop::Properties();
	prop::readProperties("Properties.xml", properties);

	createWindowClass();
	TG_VECTOR_GET_SIZE_AND_RESIZE(windowList)
	createWindow(windowList[lastSize] = new Window);
	createInstance();
	createWindowSurfaces();
	createDevice();
	prePipeline();
	initShader();
	initShaderPipes();

	tge::gmc::multiplier = (windowList[0]->height / (float)windowList[0]->width);

	createDepthTest();
	createColorResouce();
	createRenderpass();

	tge::gmc::initLight();
	tge::gmc::initCameras();
	initAllTextures();
	initDescriptors();

	allocateAllBuffers();

	for each (tge::gmc::LightActor * var in tge::gmc::lights) {
		var->updateLight();
	}
}

void startTGEngine() {

	createSwapchain();
	createFramebuffer();

	VertexBuffer vertexBuffer = {};
	vertexBuffer.maximumVertexCount = 9000000;
	createVertexBuffer(&vertexBuffer);

	IndexBuffer indexBuffer = {};
	indexBuffer.maximumIndexCount = 90000000;
	createIndexBuffer(&indexBuffer);
	createCommandBuffer();

	tge::ui::ui_scene_entity.init();

	vertexBuffer.start();
	indexBuffer.start();

	for(size_t i = 0; i < tge::gmc::models.size(); i++) {
		for (size_t j = 0; j < tge::gmc::models[i]->actors.size(); j++)
		{
			tge::gmc::models[i]->actors[j]->mesh->consume(&vertexBuffer, &indexBuffer);
		}
	}
	OUT_LV_DEBUG(tge::gmc::materiallist.size())
		for each(tge::gmc::Material * mat in tge::gmc::materiallist) {
			OUT_LV_DEBUG(mat)
				OUT_LV_DEBUG(mat->getType())
				mat->createMaterial();
		}

	index_offset = indexBuffer.indexCount;
	vertex_offset = vertexBuffer.pointCount;

	tge::ui::ui_scene_entity.draw(&indexBuffer, &vertexBuffer);

	vertexBuffer.end();
	indexBuffer.end();

	fillCommandBuffer(&indexBuffer, &vertexBuffer);

	startupCommands();
	createSemaphores();

	clock_t last_time = clock();

	uint32_t counter = 0;

	while(true) {
		windowList[0]->pollevents();
		if(windowList[0]->close_request) {
			break;
		}
		if(windowList[0]->minimized) {
			continue;
		}
		startdraw(&indexBuffer, &vertexBuffer);

		clock_t current_time = clock();
		clock_t delta = current_time - last_time;

		if(delta >= (CLOCKS_PER_SEC / 60)) {
			last_time = current_time;

			tge::ui::ui_scene_entity.update();
			vertexBuffer.pointCount = vertex_offset;
			vertexBuffer.start();
			indexBuffer.start();
			tge::ui::ui_scene_entity.draw(&indexBuffer, &vertexBuffer);
			vertexBuffer.end();
			indexBuffer.end();

			for each (tge::gmc::LightActor * var in tge::gmc::lights) {
				var->updateLight();
			}

			startSingleTimeCommand();
			vlibBufferCopy.srcOffset = vlibBufferCopy.dstOffset = vertex_offset * VERTEX_SIZE;
			vlibBufferCopy.size = vertexBuffer.pointCount * VERTEX_SIZE;
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				vertexBuffer.stag_buf.staging_buffer,
				vertexBuffer.vertex_buffer,
				1,
				&vlibBufferCopy
			);
			vlibBufferCopy.srcOffset = 0;
			vlibBufferCopy.dstOffset = index_offset * sizeof(uint32_t);
			vlibBufferCopy.size = indexBuffer.indexCount * sizeof(uint32_t);
			vkCmdCopyBuffer(
				SINGELTIME_COMMAND_BUFFER,
				indexBuffer.stag_buf.staging_buffer,
				indexBuffer.index_buffer,
				1,
				&vlibBufferCopy
			);
			endSingleTimeCommand();
		}

		submit(&indexBuffer, &vertexBuffer);
		present(&indexBuffer, &vertexBuffer);

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
	destroyIndexBuffer(&indexBuffer);
	destroyVertexBuffer(&vertexBuffer);
	destroyStagingBuffer();
	destroyFrameBuffer();
	destroySwapchain();
	destroyDescriptors();
	vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
	destroyPipeline();
	destroyRenderPass();
	destroyDepthTest();
	destroyColorResouce();
	destroyDevice();
	destroyWindows();
	destroyInstance();
}
