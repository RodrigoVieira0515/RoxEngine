#pragma once
#include <RoxEngine/renderer/CommandBuffer.h>
#include <RoxEngine/renderer/VertexArray.h>
#include <RoxEngine/renderer/Mesh.h>

namespace RoxEngine {
	class RendererPipeline
	{
	public:
		virtual void UploadMesh(const Mesh& m, std::shared_ptr<Material> mat) = 0;
		virtual std::shared_ptr<CommandBuffer> GetCmd() = 0;
	};

}
