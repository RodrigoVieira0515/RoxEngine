#include "Buffers.h"
#include <RoxEngine/renderer/RendererApi.h>
#include <Platform/Vulkan/VBuffers.h>

namespace RoxEngine {
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, void* data, size_t size)
    {
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::VertexBuffer>(layout, data, size);
		default:
			RE_CORE_ASSERT(false, fmt::format("VertexBuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
    }
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* data, size_t count)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::VULKAN:
			return std::make_shared<Vulkan::IndexBuffer>(data, count);
		default:
			RE_CORE_ASSERT(false, fmt::format("IndexBuffer::Create() not implemented for {}", ApiToStr(RendererApi::GetApi())));
		}
		return nullptr;
	}
}
