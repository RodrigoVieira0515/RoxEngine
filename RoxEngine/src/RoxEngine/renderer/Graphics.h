#pragma once
#include <RoxEngine/renderer/RendererPipeline.h>

RoxEngine::RendererPipeline* CreateRendererPipeline();
namespace RoxEngine
{
	class Graphics
	{
	public:
		static void Init();
		static void Shutdown();

		static RendererPipeline* GetRendererPipeline();
	};
}