#include "VShader.h"
#include <shaderc/shaderc.hpp>
#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>
#include <Platform/Vulkan/VRendererApi.h>
#include "spirv-cross/spirv_cross.hpp"
#include <deque>

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
		compiler_options.SetOptimizationLevel(shaderc_optimization_level_zero);
		compiler_options.SetPreserveBindings(true);

		shaderc::Compiler compiler;
		auto vertexShader = compiler.CompileGlslToSpv(vertexSrc, shaderc_vertex_shader, "-", compiler_options);
		auto fragmentShader = compiler.CompileGlslToSpv(fragmentSrc, shaderc_fragment_shader, "-", compiler_options);

		std::vector<uint32_t> vertexSpirvBin(vertexShader.begin(), vertexShader.end());
		std::vector<uint32_t> fragmentSpirvBin(fragmentShader.begin(), fragmentShader.end());

		mStages.insert({ Shader::Type::VERTEX,  api->mDevice.createShaderModule(vk::ShaderModuleCreateInfo({}, vertexSpirvBin)) });
		mStages.insert({ Shader::Type::FRAGMENT,api->mDevice.createShaderModule(vk::ShaderModuleCreateInfo({}, fragmentSpirvBin)) });
		
		spirv_cross::Compiler c(std::move(vertexSpirvBin));
		auto resources = c.get_shader_resources();

		struct UboField
		{
			
		};

		struct UboDesc {
			enum class Type {
				Boolean,
				SByte,
				UByte,
				Short,
				UShort,
				Int,
				UInt,
				Int64,
				UInt64,
				Float,
				Double,
				Struct,
			};
			struct MemberDesc {
				size_t offset;
				Type type;
			};
			size_t mSize;
			std::unordered_map<std::string, MemberDesc> mFields;
		};

		for (auto& ubo : resources.uniform_buffers)
		{
			UboDesc uboDesc;


			uint32_t binding = c.get_decoration(ubo.id, spv::DecorationBinding);
			auto& ubo_type = c.get_type(ubo.base_type_id);

			struct member_desc {
				spirv_cross::SPIRType type;
				std::string name;
				std::string dir;
				uint32_t parent_offset = 0;
			};

			std::deque<member_desc> types;
			types.push_back({ ubo_type,c.get_name(ubo_type.self)});
			uint32_t ubo_size = c.get_declared_struct_size(c.get_type(ubo.base_type_id));

			uboDesc.mSize = ubo_size;

			int spacing = 0;
			std::string name;
			while (!types.empty())
			{
				auto type = types.back(); types.pop_back();
				auto it = type.type;
				spacing++;

				for (int i = 0; i < it.member_types.size(); i++)
				{
					auto member = it.member_types[i];
					auto member_type = c.get_type(member);
					auto member_name = c.get_member_name(it.self, i);
					auto member_offset = type.parent_offset + c.get_member_decoration(it.self, i, spv::DecorationOffset);

					if (member_type.basetype == spirv_cross::SPIRType::Struct)
						types.push_back({ member_type, member_name, type.dir.size() != 0 ? (type.dir + "." + member_name) : member_name, member_offset});
					
					UboDesc::Type member_t;
					switch (member_type.basetype)
					{
						case spirv_cross::SPIRType::Boolean: member_t = UboDesc::Type::Boolean; break;
						case spirv_cross::SPIRType::SByte:   member_t = UboDesc::Type::SByte;   break;
						case spirv_cross::SPIRType::UByte:   member_t = UboDesc::Type::UByte;   break;
						case spirv_cross::SPIRType::Short:   member_t = UboDesc::Type::Short;   break;
						case spirv_cross::SPIRType::UShort:  member_t = UboDesc::Type::UShort;  break;
						case spirv_cross::SPIRType::Int:     member_t = UboDesc::Type::Int;     break;
						case spirv_cross::SPIRType::UInt:    member_t = UboDesc::Type::UInt;    break;
						case spirv_cross::SPIRType::Int64:   member_t = UboDesc::Type::Int64;   break;
						case spirv_cross::SPIRType::UInt64:  member_t = UboDesc::Type::UInt64;  break;
						case spirv_cross::SPIRType::Float:   member_t = UboDesc::Type::Float;   break;
						case spirv_cross::SPIRType::Double:  member_t = UboDesc::Type::Double;  break;
						case spirv_cross::SPIRType::Struct:  member_t = UboDesc::Type::Struct;  break;
						default: break;
					}
					uboDesc.mFields.insert({ type.dir.size() != 0 ? (type.dir + "." + member_name) : member_name,{member_offset,member_t}});
				}
			}
		}
	}
}
