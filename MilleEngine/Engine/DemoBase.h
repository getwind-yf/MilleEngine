#pragma once


#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <ctime>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <sys/stat.h>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <array>


#include "vulkan/vulkan.h"

#include "CommandLineParser.hpp" //CommandLineParser
#include "Keys.hpp"   //Keys
#include "Utils.h"   // Utils
#include "Debug.h"  //Debug
#include "UIWrapper.h"  //UIWrapper
#include "SwapChain.h" //SwapChain
#include "Buffer.h"//Buffer
#include "Device.h" //Device 
#include "Texture.h" //Texture

#include "ObjectInitializer.hpp"  //ObjectInitializer 
#include "Camera.hpp"   
#include "Benchmark.hpp" 


class DemoBase
{
private:
	std::string getWindowTitle();
	uint32_t destWidth;
	uint32_t destHeight;
	bool resizing = false;
	void handleMouseMove(int32_t x, int32_t y);
	void nextFrame();
	void updateOverlay();
	void createPipelineCache();
	void createCommandPool();
	void createSynchronizationPrimitives();
	void initSwapchain();
	void setupSwapChain();
	void createCommandBuffers();
	void destroyCommandBuffers();
	std::string shaderDir = "glsl";
protected:
	std::string getShadersPath() const;

	uint32_t frameCounter = 0;
	uint32_t lastFPS = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;
	VkInstance instance{ VK_NULL_HANDLE };
	std::vector<std::string> supportedInstanceExtensions;
	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	VkPhysicalDeviceProperties deviceProperties{};
	VkPhysicalDeviceFeatures deviceFeatures{};
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
	VkPhysicalDeviceFeatures enabledFeatures{};

	std::vector<const char*> enabledDeviceExtensions;
	std::vector<const char*> enabledInstanceExtensions;

	void* deviceCreatepNextChain = nullptr;
	VkDevice device{ VK_NULL_HANDLE };
	VkQueue queue{ VK_NULL_HANDLE };
	VkFormat depthFormat;
	VkCommandPool cmdPool{ VK_NULL_HANDLE };
	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo;
	std::vector<VkCommandBuffer> drawCmdBuffers;
	VkRenderPass renderPass{ VK_NULL_HANDLE };
	std::vector<VkFramebuffer>frameBuffers;
	uint32_t currentBuffer = 0;
	VkDescriptorPool descriptorPool{ VK_NULL_HANDLE };
	std::vector<VkShaderModule> shaderModules;
	VkPipelineCache pipelineCache{ VK_NULL_HANDLE };
	VulkanSwapChain swapChain;
	struct {
		VkSemaphore presentComplete;
		VkSemaphore renderComplete;
	} semaphores;
	std::vector<VkFence> waitFences;
	bool requiresStencil{ false };
public:
	bool prepared = false;
	bool resized = false;
	bool viewUpdated = false;
	uint32_t width = 1280;
	uint32_t height = 720;

	vent::UIWrapper ui;
	CommandLineParser commandLineParser;

	float frameTimer = 1.0f;

	vent::Benchmark benchmark;

	vent::Device* vulkanDevice;

	struct Settings {
		bool validation = false;
		bool fullscreen = false;
		bool vsync = false;
		bool overlay = true;
	} settings;

	struct {
		glm::vec2 axisLeft = glm::vec2(0.0f);
		glm::vec2 axisRight = glm::vec2(0.0f);
	} gamePadState;

	struct {
		struct {
			bool left = false;
			bool right = false;
			bool middle = false;
		} buttons;
		glm::vec2 position;
	} mouseState;

	VkClearColorValue defaultClearColor = { { 0.025f, 0.025f, 0.025f, 1.0f } };

	static std::vector<const char*> args;

	float timer = 0.0f;
	float timerSpeed = 0.25f;
	bool paused = false;

	Camera camera;

	std::string title = "Vulkan Example";
	std::string name = "vulkanExample";
	uint32_t apiVersion = VK_API_VERSION_1_0;

	struct {
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;
	} depthStencil{};

#if defined(_WIN32)
	HWND window;
	HINSTANCE windowInstance;
#endif

	DemoBase();
	virtual ~DemoBase();
	bool initVulkan();

#if defined(_WIN32)
	void setupConsole(std::string title);
	void setupDPIAwareness();
	HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
	void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	virtual VkResult createInstance();
	virtual void render() = 0;
	virtual void keyPressed(uint32_t);
	virtual void mouseMoved(double x, double y, bool& handled);
	virtual void windowResized();

	virtual void buildCommandBuffers();
	virtual void setupDepthStencil();
	virtual void setupFrameBuffer();
	virtual void setupRenderPass();
	virtual void getEnabledFeatures();
	virtual void getEnabledExtensions();

	virtual void prepare();

	VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage);

	void windowResize();
	void renderLoop();
	void drawUI(const VkCommandBuffer commandBuffer);

	void prepareFrame();
	void submitFrame();
	virtual void renderFrame();

	virtual void OnUpdateUIOverlay(vent::UIWrapper* overlay);

#if defined(_WIN32)
	virtual void OnHandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
};

#include "Launch.h"

