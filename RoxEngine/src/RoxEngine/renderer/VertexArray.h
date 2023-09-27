#pragma once
#include <memory>
#include "Buffers.h"


namespace RoxEngine {
	class VertexArray
	{
	public:
		static std::shared_ptr<VertexArray> Create();

		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> ib) = 0;
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vb) = 0;
		virtual std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() = 0;
	};
}

