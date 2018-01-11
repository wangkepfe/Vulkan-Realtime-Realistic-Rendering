#include "HelloRectangleApplication.hpp"

void HelloRectangleApplication::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, HelloRectangleApplication::onWindowResized);
}

void HelloRectangleApplication::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void HelloRectangleApplication::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	HelloRectangleApplication* app = reinterpret_cast<HelloRectangleApplication*>(glfwGetWindowUserPointer(window));
	app->recreateSwapChain();
}

std::vector<const char*> HelloRectangleApplication::getRequiredExtensions() {
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