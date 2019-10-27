#include "TGEngine.hpp"
#include "gamecontent/Light.hpp"
#include "pipeline/window/Window.hpp"

using namespace std;
using namespace tge::tex;
using namespace tge::gmc;
using namespace tge::pip;

VertexBuffer vertexBuffer;
IndexBuffer indexBuffer;

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

	initPipelines();

	multiplier = (windowList[0]->height / (float)windowList[0]->width);

	createDepthTest();
	createColorResouce();
	createRenderpass();

	initLight();
	initCameras();
	initDescriptors();

	allocateAllBuffers();

	for each (tge::gmc::LightActor * var in tge::gmc::lights) {
		var->updateLight();
	}

	vertexBuffer = {};
	vertexBuffer.maximumVertexCount = 9000000;
	createVertexBuffer(&vertexBuffer);

	indexBuffer = {};
	indexBuffer.maximumIndexCount = 90000000;
	createIndexBuffer(&indexBuffer);
	createCommandBuffer();

	vertexBuffer.start();
	indexBuffer.start();

	createSwapchain();
	createFramebuffer();

	tge::ui::ui_scene_entity.init();
}

void startTGEngine() {


	for(size_t i = 0; i < models.size(); i++) {
		for (size_t j = 0; j < models[i]->actors.size(); j++)
		{
			models[i]->actors[j]->mesh->consume(&vertexBuffer, &indexBuffer);
		}
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

			Input input = {};
			if (1 & states) {
				input.y1 = 0.01;
			}
			if (2 & states) {
				input.y1 = -0.01;
			}
			if (4 & states) {
				input.x1 = 0.01;
			}
			if (8 & states) {
				input.x1 = -0.01;
			}
			playercontroller(&input);
		}

		submit(&indexBuffer, &vertexBuffer);
		present(&indexBuffer, &vertexBuffer);

		// TESTING
		if (counter > 5) {
		//	exit(0);
		}
		counter++;
	}

	destroySemaphores();
	destroyCommandBuffer();
	destroyMemory();
	destroyIndexBuffer(&indexBuffer);
	destroyVertexBuffer(&vertexBuffer);
	destroyStagingBuffer();
	destroyFrameBuffer();
	destroySwapchain();
	destroyRenderPass();
	destroyDepthTest();
	destroyColorResouce();
	destroyDevice();
	destroyWindows();
	destroyInstance();
}
