#pragma once
#include <memory>
namespace RoxEngine {
	enum class FramebufferTexFormat {
		R8,
		RG8,
		RGB8,
		RGBA8,
		R16,
		RG16,
		RGB16,
		RGBA16,
		R32,
		RG32,
		RGB32,
		RGBA32,

		D32F,
		D32FS8U,
		D24UNS8U,
	};
	class RenderTexture
	{
	public:
		static std::shared_ptr<RenderTexture> Create(uint32_t width, uint32_t height,FramebufferTexFormat colorFormat);
		virtual ~RenderTexture() = default;
	};
}

