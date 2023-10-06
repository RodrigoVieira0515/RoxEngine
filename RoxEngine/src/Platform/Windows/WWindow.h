#pragma once
#include <RoxEngine/core/Window.h>
#include <GLFW/glfw3.h>
#include <RoxEngine/renderer/RendererApi.h>


namespace RoxEngine::Windows {
	class Window : public RoxEngine::Window
	{
	public:
		Window(WindowDesc desc);
		~Window();
		bool IsOpen() override;
		void PollEvents() override;
		void* GetNativeWindow() override;
		WindowDesc GetDesc() override;
		std::array<uint32_t, 2> GetSize() override;
		void SetEventCallback(EventCallbackFn fn) override;

		std::shared_ptr<RoxEngine::RendererApi> mRendererApi;
		GLFWwindow* mWindow;
		struct WindowData {
			Window* window;
			EventCallbackFn eventCallback;
			uint32_t width;
			uint32_t height;
			WindowDesc desc;
		};
		WindowData mData;
	};
}
