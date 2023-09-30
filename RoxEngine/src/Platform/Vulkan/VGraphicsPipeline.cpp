#include "VGraphicsPipeline.h"
#include <Platform/Vulkan/VShader.h>
#include <Platform/Vulkan/VRendererApi.h>
#include <Platform/Vulkan/VRenderPass.h>
#include <Platform/Vulkan/VMaterial.h>
#include <Platform/Vulkan/VUniformBuffer.h>
#include <RoxEngine/core/Assert.h>

namespace RoxEngine::Vulkan {
	static vk::Format ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return vk::Format::eR32Sfloat;
		case ShaderDataType::Float2:   return vk::Format::eR32G32Sfloat;
		case ShaderDataType::Float3:   return vk::Format::eR32G32B32Sfloat;
		case ShaderDataType::Float4:   return vk::Format::eR32G32B32A32Sfloat;
		case ShaderDataType::Int:      return vk::Format::eR32Sint;
		case ShaderDataType::Int2:     return vk::Format::eR32G32Sint;
		case ShaderDataType::Int3:     return vk::Format::eR32G32B32Sint;
		case ShaderDataType::Int4:     return vk::Format::eR32G32B32A32Sint;
		case ShaderDataType::Bool:     return vk::Format::eR8Uint;
		}

		RE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return vk::Format::eUndefined;
	}
	GraphicsPipeline::GraphicsPipeline(const BufferLayout& layout,std::shared_ptr<RoxEngine::Material> mat, std::shared_ptr<Framebuffer> fb)
	{
		mMat = mat;
		auto shader = (Shader*)mat->GetShader().get();
		auto api = (RendererApi*)RendererApi::Get().get();
		auto vmat = (Material*)mat.get();

		vk::PipelineLayoutCreateInfo layoutCreateInfo({}, nullptr, nullptr);
		if (shader->GetNumOfUbos() > 0)
		{
			std::vector<vk::DescriptorSetLayoutBinding> setLayoutBindings;
			setLayoutBindings.reserve(shader->mUbos.size());


			for (auto& uboDesc : shader->mUbos)
			{
				setLayoutBindings.push_back(vk::DescriptorSetLayoutBinding(uboDesc.mBinding, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eAll));
			}
			auto DescriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo({}, setLayoutBindings);
			mSetLayout = api->mDevice.createDescriptorSetLayout(DescriptorSetLayoutCreateInfo);
			auto pool = vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 1);
			vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 1, pool);

			mDescriptorPool = api->mDevice.createDescriptorPool(poolInfo);
			
			vk::DescriptorSetAllocateInfo allocInfo(mDescriptorPool, mSetLayout);
			mDescriptorSet = api->mDevice.allocateDescriptorSets(allocInfo).front();
			
			std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

			for (auto& uboDesc : shader->mUbos)
			{
				auto vUbo = (UniformBuffer*)vmat->mUbos[uboDesc.name].get();
				auto bufferInfo = vk::DescriptorBufferInfo(vUbo->mUbo, 0, uboDesc.mSize);
				writeDescriptorSets.push_back(vk::WriteDescriptorSet(mDescriptorSet, uboDesc.mBinding, 0, vk::DescriptorType::eUniformBuffer, nullptr, bufferInfo));
			}

			api->mDevice.updateDescriptorSets(writeDescriptorSets, nullptr);

			layoutCreateInfo.pSetLayouts = &mSetLayout;
			layoutCreateInfo.setLayoutCount = 1;
		}

		mLayout = api->mDevice.createPipelineLayout(layoutCreateInfo);
		

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
		shaderStages.reserve(shader->mStages.size());
		for (auto& shader : shader->mStages)
		{
			shaderStages.push_back(vk::PipelineShaderStageCreateInfo({}, ShaderTypeToVk(shader.first), shader.second,"main"));
		}
		vk::VertexInputBindingDescription vertexInputBindingDescription(0, layout.GetStride());
		std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;
		{
			uint32_t location = 0;
			for (auto& elem : layout)
			{
				vertexInputAttributeDescriptions.push_back(vk::VertexInputAttributeDescription(location, 0, ShaderDataTypeToOpenGLBaseType(elem.Type), elem.Offset));
				location++;
			}
		}

		vk::PipelineVertexInputStateCreateInfo vertexInputState({},vertexInputBindingDescription, vertexInputAttributeDescriptions);
	
		vk::PipelineInputAssemblyStateCreateInfo assemblyState({}, vk::PrimitiveTopology::eTriangleList);

		vk::PipelineViewportStateCreateInfo viewportState({}, 1, nullptr, 1, nullptr);

		vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(vk::PipelineRasterizationStateCreateFlags(),  // flags
			false,                                        // depthClampEnable
			false,                                        // rasterizerDiscardEnable
			vk::PolygonMode::eFill,                       // polygonMode
			vk::CullModeFlagBits::eBack,                  // cullMode
			vk::FrontFace::eClockwise,                    // frontFace
			false,                                        // depthBiasEnable
			0.0f,                                         // depthBiasConstantFactor
			0.0f,                                         // depthBiasClamp
			0.0f,                                         // depthBiasSlopeFactor
			1.0f                                          // lineWidth
		);
		//TODO: Add multisample support 
		vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(vk::PipelineMultisampleStateCreateFlags(),  // flags
			vk::SampleCountFlagBits::e1                 // rasterizationSamples
		);

		vk::StencilOpState                      stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
		vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(vk::PipelineDepthStencilStateCreateFlags(),  // flags
			true,                                        // depthTestEnable
			true,                                        // depthWriteEnable
			vk::CompareOp::eLessOrEqual,                 // depthCompareOp
			false,                                       // depthBoundTestEnable
			false,                                       // stencilTestEnable
			stencilOpState,                              // front
			stencilOpState                               // back
		);

		// ADD FRAMEBUFFER AS INPUT
		vk::ColorComponentFlags colorComponentFlags(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);
		
		vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(false,                   // blendEnable
			vk::BlendFactor::eZero,  // srcColorBlendFactor
			vk::BlendFactor::eZero,  // dstColorBlendFactor
			vk::BlendOp::eAdd,       // colorBlendOp
			vk::BlendFactor::eZero,  // srcAlphaBlendFactor
			vk::BlendFactor::eZero,  // dstAlphaBlendFactor
			vk::BlendOp::eAdd,       // alphaBlendOp
			colorComponentFlags      // colorWriteMask
		);
		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(vk::PipelineColorBlendStateCreateFlags(),  // flags
			false,                                     // logicOpEnable
			vk::LogicOp::eNoOp,                        // logicOp
			pipelineColorBlendAttachmentState,         // attachments
			{ { 1.0f, 1.0f, 1.0f, 1.0f } }             // blendConstants
		);

		std::array<vk::DynamicState, 2>    dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
		vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(vk::PipelineDynamicStateCreateFlags(), dynamicStates);

		vk::RenderPass renderPass = ((RenderPass*)fb->GetRenderPass().get())->mRenderPass;

		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(vk::PipelineCreateFlags(),              // flags
			shaderStages,         // stages
			&vertexInputState,    // pVertexInputState
			&assemblyState,  // pInputAssemblyState
			nullptr,                                // pTessellationState
			&viewportState,       // pViewportState
			&pipelineRasterizationStateCreateInfo,  // pRasterizationState
			&pipelineMultisampleStateCreateInfo,    // pMultisampleState
			&pipelineDepthStencilStateCreateInfo,   // pDepthStencilState
			&pipelineColorBlendStateCreateInfo,     // pColorBlendState
			&pipelineDynamicStateCreateInfo,        // pDynamicState
			mLayout,                         // layout
			renderPass                             // renderPass
		);

		vk::Result result;
		std::tie(result, mPipeline) = api->mDevice.createGraphicsPipeline(nullptr, graphicsPipelineCreateInfo);
		RE_CORE_ASSERT(result == vk::Result::eSuccess, "Failed to create graphics pipeline");
	}
}