#include "HelloTextureApplication.hpp"

void HelloTextureApplication::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, HelloTextureApplication::onWindowResized);
}

void HelloTextureApplication::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void HelloTextureApplication::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	HelloTextureApplication* app = reinterpret_cast<HelloTextureApplication*>(glfwGetWindowUserPointer(window));
	app->recreateSwapChain();
}

std::vector<const char*> HelloTextureApplication::getRequiredExtensions() {
	std::vector<const char*> extensions;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}