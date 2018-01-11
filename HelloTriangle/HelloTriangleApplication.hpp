#ifndef __HelloTriangleApplication_hpp__
#define __HelloTriangleApplication_hpp__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>

// queue families
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

// Swap chain
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// Validation layers
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);


class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:
	// const
	const int WIDTH = 800;
	const int HEIGHT = 600;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


	// Init, Mainloop, CleanUp
	void initVulkan();
	void mainLoop();
	void drawFrame();
	void cleanup();
	void cleanupSwapChain();


	// Integrating GLFW
	GLFWwindow* window;

	void initWindow();
	static void onWindowResized(GLFWwindow* window, int width, int height);
	std::vector<const char*> getRequiredExtensions();


	// Instance
	VkInstance instance;

	void createInstance();


	// Validation layers
	VkDebugReportCallbackEXT callback;

	bool checkValidationLayerSupport();
	void setupDebugCallback();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);


	// Window surface
	VkSurfaceKHR surface;

	void createSurface();


	// Physical devices(GPU) and queue families
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);


	// Logical device and queues
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	void createLogicalDevice();


	// Swap chain
	VkSwapchainKHR swapChain;

	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapChain();
	void recreateSwapChain();
	
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// Image views
	std::vector<VkImageView> swapChainImageViews;

	void createImageViews();


	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;

	void createFramebuffers();


	// Render passes
	VkRenderPass renderPass;

	void createRenderPass();


	// Graphics Pipeline
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	void createGraphicsPipeline();


	// Shader
	VkShaderModule createShaderModule(const std::vector<char>& code);
	static std::vector<char> readFile(const std::string& filename);


	// Command buffers
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	void createCommandPool();
	void createCommandBuffers();


	// Synchronization
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	void createSemaphores();
	
};

#endif // !__HelloTriangleApplication_hpp__

