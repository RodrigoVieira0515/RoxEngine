#include "VShader.h"
#include <shaderc/shaderc.hpp>
#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VRendererApi.h>
#include "../spirv-cross/spirv_cross.hpp"

namespace RoxEngine::Vulkan
{	
	shaderc_shader_kind stageToShaderc(const std::string& stage)
	{
		if (stage == "frag")
			return shaderc_fragment_shader;
		if (stage == "fragment")
			return shaderc_fragment_shader;
		if (stage == "pixel")
			return shaderc_fragment_shader;
		if (stage == "vertex")
			return shaderc_vertex_shader;
		RE_CORE_ASSERT(false, "Unknowed shader stage: {}", stage);
	}
	Shader::Type stageToShaderType(const std::string& stage)
	{
		if (stage == "frag")
			return Shader::FRAGMENT;
		if (stage == "fragment")
			return Shader::FRAGMENT;
		if (stage == "pixel")
			return Shader::FRAGMENT;
		if (stage == "vertex")
			return Shader::VERTEX;
		RE_CORE_ASSERT(false, "Unknowed shader stage: {}", stage);
	}

	vk::ShaderStageFlagBits ShaderTypeToVk(const Shader::Type& type)
	{
		switch (type)
		{
		case Shader::FRAGMENT:
			return vk::ShaderStageFlagBits::eFragment;
		case Shader::VERTEX:
			return vk::ShaderStageFlagBits::eVertex;
		default:
			break;
		}
		RE_CORE_ASSERT(false, "Unknowed shader type: {}", (int)type);
	}

	//TODO: allow serialization to skip this stage and load the spirv
	Shader::Shader(std::unordered_map<std::string, std::string>& shaderStages, std::vector<Option>& options)
		: mOptions(options)
	{
		RendererApi* api = (RendererApi*)RendererApi::Get().get();
		for (auto& [stage, source] : shaderStages)
		{
			shaderc::CompileOptions compiler_options;
			compiler_options.SetSourceLanguage(shaderc_source_language_glsl);
			compiler_options.SetForcedVersionProfile(450, shaderc_profile_core);
			compiler_options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);

			shaderc::Compiler compiler;
			auto res = compiler.CompileGlslToSpv(source, stageToShaderc(stage), "_", compiler_options);
			RE_CORE_ASSERT(res.GetCompilationStatus() == shaderc_compilation_status_success, "Failed to compile shader");

			std::vector<uint32_t> spirv_binary(res.begin(),res.end());

			vk::ShaderModuleCreateInfo createInfo({}, spirv_binary);

			mStages.insert({stageToShaderType(stage),api->mDevice.createShaderModule(createInfo) });
		}
	}

	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::vector<Option>& options)
	{
		RendererApi* api = (RendererApi*)RendererApi::Get().get();

		shaderc::CompileOptions compiler_options;
		compiler_options.SetSourceLanguage(shaderc_source_language_glsl);
		compiler_options.SetForcedVersionProfile(450, shaderc_profile_core);
		compiler_options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_0);
		compiler_options.SetGenerateDebugInfo();


		shaderc::Compiler compiler;
		auto vertexShader = compiler.CompileGlslToSpv(vertexSrc, shaderc_vertex_shader, "-", compiler_options);
		auto fragmentShader = compiler.CompileGlslToSpv(fragmentSrc, shaderc_fragment_shader, "-", compiler_options);

		std::vector<uint32_t> vertexSpirvBin(vertexShader.begin(), vertexShader.end());
		std::vector<uint32_t> fragmentSpirvBin(fragmentShader.begin(), fragmentShader.end());

		mStages.insert({ Shader::Type::VERTEX,  api->mDevice.createShaderModule(vk::ShaderModuleCreateInfo({}, vertexSpirvBin)) });
		mStages.insert({ Shader::Type::FRAGMENT,api->mDevice.createShaderModule(vk::ShaderModuleCreateInfo({}, fragmentSpirvBin)) });
		
		spirv_cross::Compiler c(std::move(vertexSpirvBin));
	}
}
