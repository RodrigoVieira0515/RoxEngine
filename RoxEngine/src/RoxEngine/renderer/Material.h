#pragma once
#include <RoxEngine/renderer/Shader.h>
#include <Platform/Vulkan/VUniformBuffer.h>
namespace RoxEngine {
	class Material
	{
	public:
		static std::shared_ptr<Material> Create(std::shared_ptr<Shader> shader);

		virtual std::shared_ptr<Shader> GetShader() = 0;
		virtual std::shared_ptr<UniformBuffer> GetUbo(const std::string& name) = 0;
	};
}
