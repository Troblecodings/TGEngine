#include "stdbase.h"
#include "Console.h"
#include "Preferences.h"
#include <fstream>
#include "Debug.h"
#include "Device.h"
#include "Swapchain.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "Files.h"
#include "Properties.h"

using namespace std;
using namespace Pipeline;


void initTGEngine() {

	nio::Properties* buf = new nio::Properties();
	nio::readProperties("Properties.xml", buf);

	vector<char*> layers = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus",
		"VK_LAYER_LUNARG_object_tracker"
	};

	Window window = {};
	window.properties = buf;
	createWindow(&window);

	cout << "Loaded window" << endl;

	Application application = {};
	application.window = &window;
	application.layers_to_enable = layers;
	application.version = buf->getInt("version").rvalue;
	createApplication(&application);

	cout << "Loaded application" << endl;

	Device main_device = {};
	main_device.prefered_format = VK_FORMAT_B8G8R8A8_UNORM;
	main_device.app = &application;
	main_device.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
	createDevice(&main_device);

	cout << "Loaded device" << endl;

	Swapchain swapchain = {};
	swapchain.app = &application;
	swapchain.device = &main_device;
	swapchain.image_count = 3;
	swapchain.image_usage_flag = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain.window = &window;
	createSwapchain(&swapchain);

	cout << "Loaded swapchain" << endl;

	Shader vertex_shader = {};
	vertex_shader.bits = VK_SHADER_STAGE_VERTEX_BIT;
	vertex_shader.name = "vert.spv";
	vertex_shader.device = main_device.device;
	createShader(&vertex_shader);

	cout << "Loaded vertex shader" << endl;

	Shader fragment_shader = {};
	fragment_shader.bits = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_shader.name = "frag.spv";
	fragment_shader.device = main_device.device;
	createShader(&fragment_shader);

	cout << "Loaded fragment shader" << endl;

	RenderPass render_pass = {};
	render_pass.device = &main_device;
	render_pass.window = &window;
	createRenderPass(&render_pass);

	cout << "Loaded render pass" << endl;

	VertexBuffer buffer = {};
	buffer.device = &main_device;
	vector<Vertex> vertecies = {
		{ { 0.0f, -0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f} },
		{ { 0.5f, 0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, 0.5f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } }
	};
	buffer.vertecies = &vertecies;
	createVertexBuffer(&buffer);

	cout << "Loaded vertex buffer" << endl;

	fillBuffer(&buffer);

	cout << "Fill vertex buffer" << endl;

	vector<VkPipelineShaderStageCreateInfo> infos = { vertex_shader.createInfo, fragment_shader.createInfo };

	Pipe line = {};
	line.shader = &infos;
	line.device = &main_device;
	line.window = &window;
	line.render_pass = &render_pass;
	line.swapchain = &swapchain;
	line.buffer = &buffer;
	createPipeline(&line);

	cout << "Loaded pipeline" << endl;

	DEPRECATED

	while (true) {
		//Window
		glfwPollEvents();
		if (glfwWindowShouldClose(window.window)) {
			cout << "CLOSE" << endl;
			break;
		}
		//Image Index
		uint32_t nextimage = 0;
		//Semaphore if Picture is avalaible
		vector<VkSemaphore> availabl_semaphor = { *line.available };
		//Gets the next Image
		handel(vkAcquireNextImageKHR(*main_device.device, *swapchain.swapchain, numeric_limits<uint32_t>::max(), availabl_semaphor[0], VK_NULL_HANDLE, &nextimage));
		
		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//Add Semaphores
		submit_info.waitSemaphoreCount = availabl_semaphor.size();
		submit_info.pWaitSemaphores = availabl_semaphor.data();
		//Set Stage FLAG
		vector<VkPipelineStageFlags> stage_flags = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.pWaitDstStageMask = stage_flags.data();

		//Command Buffers
		vector<VkCommandBuffer> buffers = { (*line.command_buffer)[nextimage] };
		submit_info.commandBufferCount = buffers.size();
		submit_info.pCommandBuffers = buffers.data();

		//Add end semaphor 
		vector<VkSemaphore> end_semaphores = { *line.end };
		submit_info.signalSemaphoreCount = end_semaphores.size();
		submit_info.pSignalSemaphores = end_semaphores.data();

		//Submit queue
		vector<VkSubmitInfo> submitinfos = { submit_info };
		handel(vkQueueSubmit(*swapchain.queue, submitinfos.size(), submitinfos.data(), VK_NULL_HANDLE));

		//Present Info
		VkPresentInfoKHR present_info = {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		//Add end semaphor
		present_info.waitSemaphoreCount = end_semaphores.size();
		present_info.pWaitSemaphores = end_semaphores.data();
		//Adding Swapchaines
		vector<VkSwapchainKHR> swapchains = { *swapchain.swapchain };
		present_info.swapchainCount = swapchains.size();
		present_info.pSwapchains = swapchains.data();

		//Adding index data
		vector<uint32_t> indices = { nextimage };
		present_info.pImageIndices = indices.data();

		//Adding result
		vector<VkResult> results = { VK_SUCCESS };
		present_info.pResults = results.data();

		//Present everything
		handel(vkQueuePresentKHR(*swapchain.queue, &present_info));
	}

	END_DEPRECATED

	destroyPipeline(&line);

	destroyRenderPass(&render_pass);

	destroyVertexBuffer(&buffer);

	destroyShader(&vertex_shader);
	destroyShader(&fragment_shader);

	destroySwapchain(&swapchain);

	destroyDevice(&main_device);

	destroyApplictaion(&application);

	destroyWindow(&window);
}

int main()
{
	initTGEngine();
	return 0;
}