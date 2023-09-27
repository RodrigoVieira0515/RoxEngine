#pragma once
#include <RoxEngine/renderer/Material.h>
#include <RoxEngine/renderer/Framebuffer.h>
#include "Buffers.h"

namespace RoxEngine {
	class GraphicsPipeline
	{
	public:
		static std::shared_ptr<GraphicsPipeline> Create(const BufferLayout& layout,std::shared_ptr<Material> mat, std::shared_ptr<Framebuffer> fb);
	};
}
