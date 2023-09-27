#include "VertexArray.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <Platform/Vulkan/VVertexArray.h>

namespace RoxEngine
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::VertexArray>();
		default:
			RE_CORE_ASSERT(false, fmt::format("CommandBuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
}