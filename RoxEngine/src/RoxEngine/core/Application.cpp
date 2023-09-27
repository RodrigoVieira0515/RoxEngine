#include "Application.h"
#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>

#include <RoxEngine/core/ImGuiLayer.h>

namespace RoxEngine {
	Application::Application(const ApplicationSpec& spec)
	{
		mMainWindow = Window::Create(spec.windowdesc);
	}
	Application::~Application()
	{
		RendererApi::Shutdown();
	}

	void Application::Run()
	{
		OnInit();
		auto api = RendererApi::Get();
		while (mMainWindow->IsOpen())
		{
			mMainWindow->PollEvents();
			#ifdef USE_IMGUI
				ImGuiLayer::NewFrame();
			#endif
			OnRender();
			#ifdef USE_IMGUI
				ImGuiLayer::EndFrame();
			#endif
			api->SwapBuffers();
		}
		OnShutdown();
	}
}