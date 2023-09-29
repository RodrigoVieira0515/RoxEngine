#pragma once
#include <RoxEngine/renderer/UniformBuffer.h>

namespace RoxEngine::Vulkan
{
	class UniformBuffer : RoxEngine::UniformBuffer
	{
	public:
		UniformBuffer(UboDesc desc);
		virtual UniformBuffer();
	};
}