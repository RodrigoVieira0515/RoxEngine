#include "RenderTexture.h"
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>

#include <Platform/Vulkan/VRenderTexture.h>

namespace RoxEngine {
	std::shared_ptr<RenderTexture> RenderTexture::Create(uint32_t width, uint32_t height, FramebufferTexFormat colorFormat)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::RenderTexture>(width, height, colorFormat);
		default:
			RE_CORE_ASSERT(false, fmt::format("RenderTexture::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
}