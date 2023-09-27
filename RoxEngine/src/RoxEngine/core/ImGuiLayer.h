#pragma once
//#define USE_IMGUI

#ifdef  USE_IMGUI
#include <imgui/imgui.h>

namespace RoxEngine {
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