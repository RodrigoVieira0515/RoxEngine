#pragma once
#include <memory>
#include <RoxEngine/renderer/Framebuffer.h>
#include <RoxEngine/renderer/GraphicsPipeline.h>
#include <RoxEngine/renderer/RenderPass.h>
#include <RoxEngine/renderer/VertexArray.h>
#include <glm/ext/vector_float4.hpp>
#include <variant>

namespace RoxEngine {
	class CommandBuffer
	{
	public:
		struct Operation {
			Operation() = default;
			~Operation() = default;
			enum Type {
				NONE,
				BIND_RENDER_PASS,
				BIND_GRAPHICS_PIPELINE,
				BIND_VERTEX_ARRAY,

				CALL_CMD,
				INLINE_CMD,

				DRAW,
			};
			#pragma region OPERATIONS_DATA
			struct opBindRp {
				std::shared_ptr<RenderPass> rp;
				std::shared_ptr<Framebuffer> fb;
				glm::vec4 clearColor;
			};
			struct opBindGp {
				std::shared_ptr<RoxEngine::GraphicsPipeline> gp;
			};

			struct opBindVa {
				std::shared_ptr<RoxEngine::VertexArray> va;
			};

			struct opDraw {
				uint32_t count;
			};

			struct opCICmd {
				std::shared_ptr<CommandBuffer> cmd;
			};

			#pragma endregion 
			uint32_t index = 0; // index of the this operation
			Type type = Type::NONE;
			
			std::variant<opBindRp, opBindGp, opBindVa, opDraw, opCICmd> data;
		};


		static std::shared_ptr<CommandBuffer> Create();

		void BindRenderPass(std::shared_ptr<RenderPass>& renderPass, std::shared_ptr<Framebuffer>& framebuffer, glm::vec4 clearColor);
		void BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline);
		void BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& va);
		void Draw(uint32_t count);
		void CallCmd(std::shared_ptr<CommandBuffer> cmd);
		void InlineCmd(std::shared_ptr<CommandBuffer> cmd);
		virtual void Execute() = 0;
	protected:
		std::vector<Operation> mOperations;
	};
}

