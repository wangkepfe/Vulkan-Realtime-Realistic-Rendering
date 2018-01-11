#ifndef __HelloTextureApplication_hpp__
#define __HelloTextureApplication_hpp__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <array>
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

// Vertex
struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

// Uniform Buffer Object: UBO
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


class HelloTextureApplication {
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

	const std::vector<Vertex> vertices = {
		{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
		{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};


	// Init, Mainloop, CleanUp
	void initVulkan();
	void mainLoop();
	void updateUniformBuffer();
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


	// Discriptor Pool / Sets
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSet();


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


	// Buffers
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);


	// Memory
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


	// Vertex Buffers
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	void createVertexBuffer();


	// Index Buffers
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	void createIndexBuffer();


	// Uniform Buffers
	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;

	void createUniformBuffer();

	
	// Synchronization
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	void createSemaphores();
	
};

#endif // !__HelloTextureApplication_hpp__

