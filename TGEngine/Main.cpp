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

using namespace std;


void debugLayerAndExtensions() {
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	vector<VkLayerProperties> props = {};
	props.resize(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, props.data());	

	
	for (int i = 0; i < layer_count; i++)
	{
		VkLayerProperties cprop = props[i];
		cout << "Found layer property " << cprop.layerName << " - " << cprop.description << endl;
		cout << "Impl_Version ";
		printVersion(cprop.implementationVersion);
		cout << endl;
		cout << "Spec_Version ";
		printVersion(cprop.specVersion);
		cout << endl;

		uint32_t ext_count = 0;
		vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, nullptr);

		vector<VkExtensionProperties> extns = {};
		extns.resize(ext_count);
		vkEnumerateInstanceExtensionProperties(cprop.layerName,&ext_count, extns.data());

		for (int g = 0; g < ext_count; g++)
		{
			cout << endl;
			VkExtensionProperties ex = extns[g];
			cout << g + 1 << ". " << ex.extensionName << " - Version: ";
			printVersion(ex.specVersion);
			cout << endl;
		}

		cout << endl;
	}

	cout << "Extensions: " << endl;

	uint32_t ext_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);

	vector<VkExtensionProperties> extns = {};
	extns.resize(ext_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extns.data());

	for (int g = 0; g < ext_count; g++)
	{
		cout << endl;
		VkExtensionProperties ex = extns[g];
		cout << g + 1 << ". " << ex.extensionName << " - Version: ";
		printVersion(ex.specVersion);
		cout << endl;
	}

	cout << endl;
}

using namespace Options;
using namespace Pipeline;

void initTGEngine() {

	vector<char*> layers = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_NV_optimus"
	};

	Window window = {};
	window.autosize = true;
	window.title = "title";
	createWindow(&window);

	Application application = {};
	application.window = &window;
	application.layers_to_enable = layers;
	application.version = VK_MAKE_VERSION(0, 0, 1);
	createApplication(&application);

	Device main_device = {};
	main_device.prefered_format = VK_FORMAT_B8G8R8A8_UNORM;
	main_device.app = &application;
	main_device.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
	createDevice(&main_device);

	Swapchain swapchain = {};
	swapchain.app = &application;
	swapchain.device = &main_device;
	swapchain.image_count = 3;
	swapchain.image_usage_flag = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain.sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain.present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
	swapchain.window = &window;
	createSwapchain(&swapchain);

	Shader vertex_shader = {};
	vertex_shader.bits = VK_SHADER_STAGE_VERTEX_BIT;
	vertex_shader.name = "vert.spv";
	vertex_shader.device = main_device.device;
	createShader(&vertex_shader);

	Shader fragment_shader = {};
	fragment_shader.bits = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_shader.name = "frag.spv";
	fragment_shader.device = main_device.device;
	createShader(&fragment_shader);

	RenderPass render_pass = {};
	render_pass.device = &main_device;
	render_pass.window = &window;
	createRenderPass(&render_pass);

	Pipe line = {};
	line.shader = { vertex_shader.createInfo, fragment_shader.createInfo };
	line.device = main_device.device;
	line.window = &window;
	line.render_pass = &render_pass;
	createPipeline(&line);

	while (true) {
		glfwPollEvents();
		if (glfwWindowShouldClose(window.window))break;
		uint32_t nextimage = 0;
		handel(vkAcquireNextImageKHR(*main_device.device, *swapchain.swapchain, numeric_limits<uint64_t>::max(), *line.available, VK_NULL_HANDLE, &nextimage));

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = line.available;
		vector<VkPipelineStageFlags> stage_flags = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.pWaitDstStageMask = stage_flags.data();
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &(line.command_buffer[nextimage]);
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = line.end;

		handel(vkQueueSubmit(*swapchain.queue, nextimage, &submit_info, VK_NULL_HANDLE));

		VkPresentInfoKHR present_info = {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = line.end;
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swapchain.swapchain;
		present_info.pImageIndices = &nextimage;
		present_info.pResults = nullptr;

		handel(vkQueuePresentKHR(*swapchain.queue, &present_info));
	}

	destroyPipeline(&line);

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