#include <RoxEngine/RoxEngine.h>
#include <RoxEngine/core/ImGuiLayer.h>
#include <RoxEngine/renderer/RenderPass.h>
#include <RoxEngine/renderer/Framebuffer.h>
#include <RoxEngine/renderer/Material.h>
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <RoxEngine/renderer/Buffers.h>
#include <RoxEngine/renderer/CommandBuffer.h>
#include <RoxEngine/renderer/RendererApi.h>
#include <Platform/Vulkan/VRendererApi.h>
#include <RoxEngine/renderer/RendererPipeline.h>
#include <RoxEngine/renderer/Graphics.h>
#include <iostream>
#include <chrono>
#include <glm/gtx/string_cast.hpp>

using namespace RoxEngine;

/*
	1º. pass: Organizes the meshes into octree // executed every uploadMesh() or similar
	2º. pass: Sees what visible for X camera
	3º. pass: Organizes the meshes info for batch rendering and/or instancing (For X camera)
	4º. pass: Renders to X camera
	5º. pass: Go back to step 3 if is needed to render to more than 1 Camera
*/
class RendererPipeline1 : public RendererPipeline
{
public:
	std::shared_ptr<CommandBuffer> cmd;
	std::shared_ptr<Material> Mat;
	std::shared_ptr<Framebuffer> fb;
	std::shared_ptr<GraphicsPipeline> pipeline;
		 
	std::shared_ptr<VertexArray> va;
	BufferLayout bufferLayout;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	bool updated;

	RendererPipeline1() {
		cmd = CommandBuffer::Create();
		auto renderPass = RenderPass::Create({ {FramebufferTexFormat::RGB8, StoreLoadOp::CLEAR, StoreLoadOp::LOAD} }, { Subpass(0, std::vector<uint32_t>(), { 0 }, std::vector<uint32_t>()) });
		fb = Framebuffer::Create(800, 800, { FramebufferTexFormat::RGB8 }, renderPass);
		bufferLayout = { {"mPos", ShaderDataType::Float3},{"mUv", ShaderDataType::Float2},{"mNormal", ShaderDataType::Float3} };
	}

	void UploadMesh(const Mesh& m, std::shared_ptr<Material> mat) override {
		if (!Mat) {
			Mat = mat;
			pipeline = GraphicsPipeline::Create(bufferLayout, Mat, fb);
		}

		auto& mvertices = m.GetReadOnlyVertices();
		auto& mindices = m.GetReadOnlyIndices();

		if (mvertices.size() == 0)
			return;

		vertices.insert(vertices.end(), mvertices.begin(), mvertices.end());
		indices.insert(indices.end(), mindices.begin(), mindices.end());

		// radius
		auto boundingBoxR = m.FindBoundingBox();
		// no transform yet
		glm::vec3 position = { 0,0,0 };
		updated = true;
	}
	std::shared_ptr<CommandBuffer> GetCmd() override {
		if (updated) {
			auto vb = VertexBuffer::Create(bufferLayout, vertices.data(), sizeof(Vertex) * vertices.size());
			auto ib = IndexBuffer::Create(indices.data(), indices.size());

			va = VertexArray::Create();
			va->AddVertexBuffer(vb);
			va->SetIndexBuffer(ib);
			updated = false;
		}
		cmd->Reset();
		if (va) {
			cmd->BindRenderPass(fb->GetRenderPass(), fb, glm::vec4(1, 0.5, 0.5, 1));
			cmd->BindGraphicsPipeline(pipeline);
			cmd->BindVertexArray(va);
			cmd->Draw(vertices.size());
			cmd->UnbindVertexArray();
			cmd->UnbindGraphicsPipeline();
			cmd->UnbindRenderPass();
		}
		cmd->BlitFramebuffers(fb, RendererApi::Get()->GetFramebuffer());

		return cmd;
	}
};

class SandboxApp : public Application
{
public:
	SandboxApp() : Application(ApplicationSpec{ WindowDesc{"SandboxApp", 800,800}}) {
	}
	virtual ~SandboxApp() {

	}
	void OnInit() {
#ifdef USE_IMGUI
		ImGuiLayer::Init();
#endif
		Mesh m(
			{
				Vertex(glm::vec3(-0.5,-0.5, 0.0)),
				Vertex(glm::vec3( 0.5,-0.5, 0.0)),
				Vertex(glm::vec3( 0.0, 0.5, 0.0)),
			}, 
			{
				0,1,2
			}
			);
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
		bool ok = ubo->Set("yellow", 1);

		Graphics::GetRendererPipeline()->UploadMesh(m, mat);
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
#ifdef USE_IMGUI
		ImGui::Begin("Hello World");
		ImGui::End();
#endif
	}
	void OnEvent(Event& event) {
		RE_CORE_INFO("{}", event.ToString());
	}
	void OnShutdown() {
	}
};

Application* createApp() {
	return new SandboxApp();
}
RendererPipeline* CreateRendererPipeline() {
	return new RendererPipeline1();
}