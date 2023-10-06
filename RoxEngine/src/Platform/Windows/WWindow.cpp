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
		
		mData.desc = desc;

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

		glfwSetWindowUserPointer(mWindow, &mData);

		{
			glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.width = width;
					data.height = height;

					WindowResizeEvent event(width, height);
					event.window = data.window;

					glfwSetWindowSize(window, width, height);

					if (data.eventCallback) data.eventCallback(event);

				});

			glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					WindowCloseEvent event;
					event.window = data.window;
					if (data.eventCallback) data.eventCallback(event);
				});

			glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						if (data.eventCallback) data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						if (data.eventCallback) data.eventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						if (data.eventCallback) data.eventCallback(event);
						break;
					}
					}
				});

			glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keycode)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					KeyTypedEvent event(keycode);
					if (data.eventCallback) data.eventCallback(event);
				});

			glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						if (data.eventCallback) data.eventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						if (data.eventCallback) data.eventCallback(event);
						break;
					}
					}
				});

			glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseScrolledEvent event((float)xOffset, (float)yOffset);
					if (data.eventCallback) data.eventCallback(event);
				});

			glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseMovedEvent event((float)xPos, (float)yPos);
					if (data.eventCallback) data.eventCallback(event);
				});
		}
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
		return mData.desc;
	}
	std::array<uint32_t, 2> Window::GetSize()
	{
		int width, height;
		glfwGetWindowSize(mWindow, &width, &height);

		return { static_cast<uint32_t>(width),static_cast<uint32_t>(height)};
	}
	void Window::SetEventCallback(EventCallbackFn fn)
	{
		mData.eventCallback = fn;
	}
}