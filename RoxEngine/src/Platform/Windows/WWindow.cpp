#include "WWindow.h"

#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>

#include <RoxEngine/renderer/RendererApi.h>

namespace RoxEngine::Windows {
	static bool glfwInitialized = false;

	void glfwErrorCallback(int errorCode, const char* desc)
	{
		RE_ERROR("GLFW ERROR({}):{}", errorCode, desc);
	}

	Window::Window(WindowDesc desc)
	{
		RE_CORE_ASSERT(!glfwInitialized, "RoxEngine doesn't support multiple Windows");
		
		mDesc = desc;

		if (!glfwInitialized) {
			RE_CORE_ASSERT(glfwInit() == GLFW_TRUE, "Could not initialize GLFW3");
			glfwInitialized = true;
		} 
		auto api = RendererApi::VULKAN;
		if (api == RendererApi::VULKAN)
		{
			RE_VERIFY(glfwVulkanSupported(), "Vulkan is not supported on current device");
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(desc.width, desc.height, desc.name, nullptr, nullptr);
		
		glfwMakeContextCurrent(mWindow);
		
		mRendererApi = RoxEngine::RendererApi::Init(RendererApi::VULKAN,this);

		glfwShowWindow(mWindow);
	}
	Window::~Window() {
		glfwTerminate();
	}
	bool Window::IsOpen()
	{
		return !glfwWindowShouldClose(mWindow);
	}
	void Window::PollEvents()
	{
		glfwPollEvents();
	}
	void* Window::GetNativeWindow()
	{
		return mWindow;
	}
	WindowDesc Window::GetDesc()
	{
		return mDesc;
	}
	std::array<uint32_t, 2> Window::GetSize()
	{
		int width, height;
		glfwGetWindowSize(mWindow, &width, &height);

		return { static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
	}
}