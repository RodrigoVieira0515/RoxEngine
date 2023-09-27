#include "RendererApi.h"
#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>

#include <Platform/Vulkan/VRendererApi.h>

namespace RoxEngine {

	static RendererApi::API sApi;
	static bool initialized = false;
	static std::shared_ptr<RendererApi> sRendererApi;

	const char* ApiToStr(RendererApi::API api)
	{
		switch (api)
		{
		case RendererApi::VULKAN:
			return "RendererApi::VULKAN";
		default:
			RE_CORE_ASSERT(false, "ApiToStr(API)");
		}
	}

	std::shared_ptr<RendererApi> RendererApi::Init(API api, Window* window)
	{
		sApi = api;
		switch (api)
		{
		case RoxEngine::RendererApi::VULKAN:
			sRendererApi = std::make_shared<Vulkan::RendererApi>(window);
			break;
		default:
			RE_CORE_ASSERT(false, fmt::format("RendererApi::Init() not implemented for {}", ApiToStr(api)));
		}

		initialized = true;
		return sRendererApi;
	}

	void RendererApi::Shutdown()
	{
		RE_CORE_ASSERT(sRendererApi != nullptr,"RendererApi::Shutdown() without ever calling RendererApi::Init()");
		sRendererApi.reset();
		initialized = false;
	}

	std::shared_ptr<RendererApi> RendererApi::Get()
	{
		RE_CORE_ASSERT(sRendererApi != nullptr, "RendererApi::Get() without ever calling RendererApi::Init()");
		return sRendererApi;
	}

	RendererApi::API RendererApi::GetApi()
	{
		RE_CORE_ASSERT(initialized == true, "RendererApi::GetApi() without ever calling RendererApi::Init()");
		return sApi;
	}
}