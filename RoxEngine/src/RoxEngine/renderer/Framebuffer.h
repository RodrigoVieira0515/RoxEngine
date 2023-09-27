#pragma once
#include <memory>
#include <RoxEngine/renderer/RenderTexture.h>
#include <vector>
#include "RenderPass.h"
#include <glm/ext/vector_int2.hpp>

namespace RoxEngine {
	class Framebuffer
	{
	public:
		static std::shared_ptr<Framebuffer> Create(uint32_t width, uint32_t height, std::vector<FramebufferTexFormat> attachments, std::shared_ptr<RoxEngine::RenderPass> renderpass);
		virtual std::shared_ptr<RenderPass> GetRenderPass() = 0;
		virtual glm::ivec2 GetSize() = 0;
		virtual ~Framebuffer() = default;
	};
}

