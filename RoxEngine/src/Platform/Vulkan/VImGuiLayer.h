#pragma once
#include <cstdint>
#include <RoxEngine/core/ImGuiLayer.h>

#ifdef  USE_IMGUI
#include <imgui/imgui.h>
namespace RoxEngine::Vulkan {
	class ImGuiLayer
	{
	public:
		static void Init();
		static void Shutdown();
		static void NewFrame();
		static void EndFrame();
	};
}
#endif