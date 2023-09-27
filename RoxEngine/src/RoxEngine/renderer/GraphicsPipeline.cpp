#include "GraphicsPipeline.h"
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>
#include <Platform/Vulkan/VGraphicsPipeline.h>

namespace RoxEngine {
    std::shared_ptr<GraphicsPipeline> GraphicsPipeline::Create(const BufferLayout& layout, std::shared_ptr<Material> mat,std::shared_ptr<Framebuffer> fb)
    {
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::GraphicsPipeline>(layout,mat,fb);
		default:
			RE_CORE_ASSERT(false, fmt::format("GraphicsPipeline::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
    }
}