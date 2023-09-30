#include <RoxEngine/RoxEngine.h>
#include <RoxEngine/core/ImGuiLayer.h>
#include <RoxEngine/renderer/RenderPass.h>
#include <RoxEngine/renderer/Framebuffer.h>
#include <RoxEngine/renderer/Material.h>
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <RoxEngine/renderer/Buffers.h>
#include <RoxEngine/renderer/CommandBuffer.h>
#include <RoxEngine/renderer/RendererApi.h>

#include <Platform/vulkan/VRendererApi.h>

#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/MachineIndependent/reflection.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <shaderc/shaderc.hpp>
using namespace RoxEngine;

TBuiltInResource InitResources()
{
	TBuiltInResource Resources;

	Resources.maxLights = 32;
	Resources.maxClipPlanes = 6;
	Resources.maxTextureUnits = 32;
	Resources.maxTextureCoords = 32;
	Resources.maxVertexAttribs = 64;
	Resources.maxVertexUniformComponents = 4096;
	Resources.maxVaryingFloats = 64;
	Resources.maxVertexTextureImageUnits = 32;
	Resources.maxCombinedTextureImageUnits = 80;
	Resources.maxTextureImageUnits = 32;
	Resources.maxFragmentUniformComponents = 4096;
	Resources.maxDrawBuffers = 32;
	Resources.maxVertexUniformVectors = 128;
	Resources.maxVaryingVectors = 8;
	Resources.maxFragmentUniformVectors = 16;
	Resources.maxVertexOutputVectors = 16;
	Resources.maxFragmentInputVectors = 15;
	Resources.minProgramTexelOffset = -8;
	Resources.maxProgramTexelOffset = 7;
	Resources.maxClipDistances = 8;
	Resources.maxComputeWorkGroupCountX = 65535;
	Resources.maxComputeWorkGroupCountY = 65535;
	Resources.maxComputeWorkGroupCountZ = 65535;
	Resources.maxComputeWorkGroupSizeX = 1024;
	Resources.maxComputeWorkGroupSizeY = 1024;
	Resources.maxComputeWorkGroupSizeZ = 64;
	Resources.maxComputeUniformComponents = 1024;
	Resources.maxComputeTextureImageUnits = 16;
	Resources.maxComputeImageUniforms = 8;
	Resources.maxComputeAtomicCounters = 8;
	Resources.maxComputeAtomicCounterBuffers = 1;
	Resources.maxVaryingComponents = 60;
	Resources.maxVertexOutputComponents = 64;
	Resources.maxGeometryInputComponents = 64;
	Resources.maxGeometryOutputComponents = 128;
	Resources.maxFragmentInputComponents = 128;
	Resources.maxImageUnits = 8;
	Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
	Resources.maxCombinedShaderOutputResources = 8;
	Resources.maxImageSamples = 0;
	Resources.maxVertexImageUniforms = 0;
	Resources.maxTessControlImageUniforms = 0;
	Resources.maxTessEvaluationImageUniforms = 0;
	Resources.maxGeometryImageUniforms = 0;
	Resources.maxFragmentImageUniforms = 8;
	Resources.maxCombinedImageUniforms = 8;
	Resources.maxGeometryTextureImageUnits = 16;
	Resources.maxGeometryOutputVertices = 256;
	Resources.maxGeometryTotalOutputComponents = 1024;
	Resources.maxGeometryUniformComponents = 1024;
	Resources.maxGeometryVaryingComponents = 64;
	Resources.maxTessControlInputComponents = 128;
	Resources.maxTessControlOutputComponents = 128;
	Resources.maxTessControlTextureImageUnits = 16;
	Resources.maxTessControlUniformComponents = 1024;
	Resources.maxTessControlTotalOutputComponents = 4096;
	Resources.maxTessEvaluationInputComponents = 128;
	Resources.maxTessEvaluationOutputComponents = 128;
	Resources.maxTessEvaluationTextureImageUnits = 16;
	Resources.maxTessEvaluationUniformComponents = 1024;
	Resources.maxTessPatchComponents = 120;
	Resources.maxPatchVertices = 32;
	Resources.maxTessGenLevel = 64;
	Resources.maxViewports = 16;
	Resources.maxVertexAtomicCounters = 0;
	Resources.maxTessControlAtomicCounters = 0;
	Resources.maxTessEvaluationAtomicCounters = 0;
	Resources.maxGeometryAtomicCounters = 0;
	Resources.maxFragmentAtomicCounters = 8;
	Resources.maxCombinedAtomicCounters = 8;
	Resources.maxAtomicCounterBindings = 1;
	Resources.maxVertexAtomicCounterBuffers = 0;
	Resources.maxTessControlAtomicCounterBuffers = 0;
	Resources.maxTessEvaluationAtomicCounterBuffers = 0;
	Resources.maxGeometryAtomicCounterBuffers = 0;
	Resources.maxFragmentAtomicCounterBuffers = 1;
	Resources.maxCombinedAtomicCounterBuffers = 1;
	Resources.maxAtomicCounterBufferSize = 16384;
	Resources.maxTransformFeedbackBuffers = 4;
	Resources.maxTransformFeedbackInterleavedComponents = 64;
	Resources.maxCullDistances = 8;
	Resources.maxCombinedClipAndCullDistances = 8;
	Resources.maxSamples = 4;
	Resources.maxMeshOutputVerticesNV = 256;
	Resources.maxMeshOutputPrimitivesNV = 512;
	Resources.maxMeshWorkGroupSizeX_NV = 32;
	Resources.maxMeshWorkGroupSizeY_NV = 1;
	Resources.maxMeshWorkGroupSizeZ_NV = 1;
	Resources.maxTaskWorkGroupSizeX_NV = 32;
	Resources.maxTaskWorkGroupSizeY_NV = 1;
	Resources.maxTaskWorkGroupSizeZ_NV = 1;
	Resources.maxMeshViewCountNV = 4;

	Resources.limits.nonInductiveForLoops = 1;
	Resources.limits.whileLoops = 1;
	Resources.limits.doWhileLoops = 1;
	Resources.limits.generalUniformIndexing = 1;
	Resources.limits.generalAttributeMatrixVectorIndexing = 1;
	Resources.limits.generalVaryingIndexing = 1;
	Resources.limits.generalSamplerIndexing = 1;
	Resources.limits.generalVariableIndexing = 1;
	Resources.limits.generalConstantMatrixVectorIndexing = 1;

	return Resources;
}


class SandboxApp : public Application
{
public:
	std::shared_ptr<CommandBuffer> cmd;
	std::shared_ptr<CommandBuffer> cmd2;
	std::shared_ptr<Framebuffer> fb;

	SandboxApp() : Application(ApplicationSpec{ WindowDesc{"SandboxApp", 800,800}}) {
	}
	virtual ~SandboxApp() {

	}
	void OnInit() {

#ifdef USE_IMGUI
		ImGuiLayer::Init();
#endif
		float vertices[3*3] = { -0.5f, -0.5f,0.0f, // Vertex 0 (x, y)
							    0.5f, -0.5f,0.0f, // Vertex 1 (x, y)
							    0.0f,  0.5f,0.0f };
		uint32_t indices[3] = { 0,1,2 };
		static auto va = VertexArray::Create();
		static auto vb = VertexBuffer::Create({ {"mPos", ShaderDataType::Float3} }, vertices, sizeof(float) * 9);
		static auto ib = IndexBuffer::Create(indices, 3);

		va->SetIndexBuffer(ib);
		va->AddVertexBuffer(vb);


		static auto renderPass = RenderPass::Create({ {FramebufferTexFormat::RGB8, StoreLoadOp::CLEAR, StoreLoadOp::LOAD} }, { Subpass(0, std::vector<uint32_t>(), { 0 }, std::vector<uint32_t>()) });
		fb = Framebuffer::Create(800, 800, { FramebufferTexFormat::RGB8 }, renderPass);

		static auto shader = Shader::Create(
			R"(
				layout(location = 0) in vec3 aPos;

				void main() {
					gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
				}
			)", 
			R"(
				layout(location = 0) out vec4 FragColor;

				layout(binding = 4, std140) uniform ubo{
					bool yellow;
				};

				void main() {
					if(yellow == true) {
						FragColor = vec4(1,1,0,1);
					}
					else {
						FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
					}
				}
			)", 
			{});
		static auto mat = Material::Create(shader);
		auto ubo = mat->GetUbo("ubo");

		static auto pipeline = GraphicsPipeline::Create(vb->GetLayout(),mat, fb);
		bool ok = ubo->Set("yellow", 1);

		cmd = CommandBuffer::Create();
		
		//cmd->BeginWrite();
		cmd->BindRenderPass(renderPass, fb, glm::vec4(1,0.5,0.5,1));
		cmd->BindGraphicsPipeline(pipeline);
		cmd->BindVertexArray(va);
		cmd->Draw(3);
		//cmd->EndWrite();
		cmd->Execute();

		((Vulkan::RendererApi*)RendererApi::Get().get())->mSrcFb = fb.get();
	}
	void OnRender() {
		// Draw Gui
#ifdef USE_IMGUI
		{
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace", nullptr, window_flags);
			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("VulkanAppDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			OnImGuiDraw();

			ImGui::End();
		}
#endif
	}
	void OnImGuiDraw() {
	}
	void OnShutdown() {
	}
};

Application* createApp() {
	return new SandboxApp();
}