#pragma once
#include <RoxEngine/renderer/Shader.h>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include <RoxEngine/renderer/UniformBuffer.h>

namespace RoxEngine::Vulkan {
	vk::ShaderStageFlagBits ShaderTypeToVk(const Shader::Type& type);

	class Shader : public RoxEngine::Shader
	{
	public:
		Shader(std::unordered_map<std::string,std::string>& shaderStages, std::vector<Option>& options);
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::vector<Option>& options);

		void GetReflection(std::vector<uint32_t>& spirv);

		uint32_t GetNumOfUbos();

		std::unordered_map<Shader::Type, vk::ShaderModule> mStages;
		std::vector<UniformBuffer::UboDesc> mUbos;
		std::vector<Option> mOptions;
	};
}

