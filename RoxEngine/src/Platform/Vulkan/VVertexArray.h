#pragma once
#include <RoxEngine/renderer/VertexArray.h>

namespace RoxEngine::Vulkan
{
	class VertexArray : public RoxEngine::VertexArray
	{
	public:
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> ib) override;
		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vb) override;
		std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override;
		std::shared_ptr<IndexBuffer> GetIndexBuffer() override;

		std::vector<std::shared_ptr<VertexBuffer>> mBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};
}

