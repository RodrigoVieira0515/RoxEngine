#include "CommandBuffer.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VCommandBuffer.h>

namespace RoxEngine {
	std::shared_ptr<CommandBuffer> CommandBuffer::Create()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::CommandBuffer>();
		default:
			RE_CORE_ASSERT(false, fmt::format("CommandBuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
}
