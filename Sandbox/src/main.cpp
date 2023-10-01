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

using namespace RoxEngine;

// Meant for dynamic metadata not static, like information on a mesh from a custom renderer pipeline
class Metadata {
public:
	~Metadata() {
		for (auto& [key, data] : mData)
		{
			data.mDelete(data.ptr);
		}
	}

	template<typename T>
	void Add(T&& val) {
		RE_CORE_ASSERT(!Has<T>(), "Adding metadata that already exists");
		mData.insert({typeid(T).name(),{new T(val), [](void* raw_value) {T* value = (T*)raw_value; delete raw_value; }}});
	}
	template<typename T>
	void AddOrReplace(T&& val) {
		if (Has<T>())
			Remove<T>();
		Add<T>(std::move(val));
	}
	template<typename T>
	bool Has() {
		return mData.find(typeid(T).name()) != mData.end();
	}
	template<typename T>
	T* Get() {
		RE_CORE_ASSERT(Has<T>(), "Getting metadata that doesn't exist");
		return (T*)mData.at(typeid(T).name()).ptr;
	}

	template<typename T>
	void Remove() {
		RE_CORE_ASSERT(Has<T>(), "Removing metadata that doesn't exist");
		delete Get<T>();
		mData.erase(typeid(T).name());
	}

private:
	struct Data {
		void* ptr;
		std::function<void(void*)> mDelete;
	};

	std::unordered_map<std::string, Data> mData;
};

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv = { 0,0 };
	glm::vec3 normal = { 0,0,0};
};

class Mesh {
public:
	// Depending on the type some functions can't be called
	enum Type {
		STATIC,
		DYNAMIC,
	};

	Mesh(Type type = Type::DYNAMIC, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : type(type) {
		mVertices = vertices;
		mIndices = indices;
	}
	
	const std::vector<Vertex>& GetReadOnlyVertices() {
		return mVertices;
	}
	const std::vector<uint32_t>& GetReadOnlyIndices() {
		return mIndices;
	}

	std::vector<Vertex>& GetVertices() {
		RE_CORE_ASSERT(type == Type::DYNAMIC, "Cannot get vertices of static mesh.");
		return mVertices;
	}
	std::vector<uint32_t>& GetIndices() {
		RE_CORE_ASSERT(type == Type::DYNAMIC, "Cannot get indices of static mesh.");
		return mIndices;
	}


	Metadata metadata;
	const Type type;
private:
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;
};

class RendererPipeline1 : public RendererPipeline
{
public:
	std::shared_ptr<CommandBuffer> cmd;

	RendererPipeline1() {
		cmd = CommandBuffer::Create();
	}

	void UploadMesh(std::shared_ptr<VertexArray> va) override {

	}
	std::shared_ptr<CommandBuffer> GetCmd() override {
		return cmd;
	}
};

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
		
		cmd->BindRenderPass(renderPass, fb, glm::vec4(1,0.5,0.5,1));
		cmd->BindGraphicsPipeline(pipeline);
		cmd->BindVertexArray(va);
		cmd->Draw(3);
		cmd->Execute();
	}
	void OnRender() {
		((RendererPipeline1*)Graphics::GetRendererPipeline())->cmd->BlitFramebuffers(fb, RendererApi::Get()->GetFramebuffer());
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
	void OnShutdown() {
	}
};

Application* createApp() {
	return new SandboxApp();
}
RendererPipeline* CreateRendererPipeline() {
	return new RendererPipeline1();
}