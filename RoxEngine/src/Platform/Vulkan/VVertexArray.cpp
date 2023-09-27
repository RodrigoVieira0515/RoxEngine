#include "VVertexArray.h"

namespace RoxEngine::Vulkan
{
	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> ib)
	{
		mIndexBuffer = ib;
	}

	void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vb)
	{
		mBuffers.push_back(vb);
	}

	std::vector<std::shared_ptr<VertexBuffer>>& VertexArray::GetVertexBuffers()
	{
		return mBuffers;
	}

	std::shared_ptr<IndexBuffer> VertexArray::GetIndexBuffer()
	{
		return mIndexBuffer;
	}
}