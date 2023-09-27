#pragma once
#include <RoxEngine/renderer/Shader.h>
#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace RoxEngine::Vulkan {
	vk::ShaderStageFlagBits ShaderTypeToVk(const Shader::Type& type);

	class Shader : public RoxEngine::Shader
	{
	public:
		Shader(std::unordered_map<std::string,std::string>& shaderStages, std::vector<Option>& options);
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::vector<Option>& options);

		std::unordered_map<Shader::Type, vk::ShaderModule> mStages;
		std::vector<Option> mOptions;
	};
}

