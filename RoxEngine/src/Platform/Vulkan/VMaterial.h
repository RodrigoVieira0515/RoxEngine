#pragma once
#include <RoxEngine/renderer/Material.h>
#include <Platform/Vulkan/VShader.h>
#include <RoxEngine/renderer/UniformBuffer.h>

namespace RoxEngine::Vulkan {
	class Material : public RoxEngine::Material
	{
	public:
		Material(std::shared_ptr<RoxEngine::Shader> shader);
		std::shared_ptr<RoxEngine::Shader> GetShader();
		std::shared_ptr<RoxEngine::UniformBuffer> GetUbo(const std::string& name);
		std::unordered_map<std::string, std::shared_ptr<RoxEngine::UniformBuffer>> mUbos;
		std::shared_ptr<Shader> mShader;
	};
}
