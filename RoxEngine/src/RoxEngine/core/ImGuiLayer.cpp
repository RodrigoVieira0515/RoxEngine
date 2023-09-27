#include "ImGuiLayer.h"
#include <RoxEngine/renderer/RendererApi.h>
#include "Assert.h"
#include <Platform/Vulkan/VImGuiLayer.h>
#ifdef  USE_IMGUI
namespace RoxEngine {
	void ImGuiLayer::Init()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			Vulkan::ImGuiLayer::Init();
			break;
		default:
			RE_CORE_ASSERT(false, fmt::format("ImGuiLayer::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
	}
	void ImGuiLayer::Shutdown()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			Vulkan::ImGuiLayer::Shutdown();
			break;
		default:
			RE_CORE_ASSERT(false, fmt::format("ImGuiLayer::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
	}
	void ImGuiLayer::NewFrame()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			Vulkan::ImGuiLayer::NewFrame();
			break;
		default:
			RE_CORE_ASSERT(false, fmt::format("ImGuiLayer::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
	}
	void ImGuiLayer::EndFrame()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			Vulkan::ImGuiLayer::EndFrame();
			break;
		default:
			RE_CORE_ASSERT(false, fmt::format("ImGuiLayer::Init() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
	}
}
#endif