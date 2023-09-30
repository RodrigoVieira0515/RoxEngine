#pragma once
#include <memory>
#include <RoxEngine/core/Window.h>
#include <RoxEngine/renderer/Framebuffer.h>
namespace RoxEngine {
	class RendererApi
	{
	public:
		enum API {
			VULKAN = 1
		};

		static std::shared_ptr<RendererApi> Init(API api, Window* window);
		static void Shutdown();

		static std::shared_ptr<RendererApi> Get();
		static API GetApi();

		virtual void SwapBuffers() = 0;
		virtual std::shared_ptr<Framebuffer> GetFramebuffer() = 0;
	};
	const char* ApiToStr(RendererApi::API api);
}
