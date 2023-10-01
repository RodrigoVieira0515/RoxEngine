#pragma once
#include <RoxEngine/renderer/CommandBuffer.h>
#include <RoxEngine/renderer/VertexArray.h>
namespace RoxEngine {
	class RendererPipeline
	{
	public:
		virtual void UploadMesh(std::shared_ptr<VertexArray> va) = 0;
		virtual std::shared_ptr<CommandBuffer> GetCmd() = 0;
	};

}
