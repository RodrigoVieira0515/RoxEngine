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

				UNBIND_RENDER_PASS,
				UNBIND_GRAPHICS_PIPELINE,
				UNBIND_VERTEX_ARRAY,

				CALL_CMD,
				INLINE_CMD,

				BLIT_FB,

				DRAW,

				// will just use a std::function pack you custom code into it
				RAW_CALL
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
			struct opBlitFb {
				std::shared_ptr<Framebuffer> src;
				std::shared_ptr<Framebuffer> dst;
			};
			struct opRawCall {
				std::function<void(RoxEngine::CommandBuffer*,void*)> fn;
			};

			#pragma endregion 
			uint32_t index = 0; // index of the this operation
			Type type = Type::NONE;
			
			std::variant<opBindRp, opBindGp, opBindVa, opDraw, opCICmd, opBlitFb, opRawCall, int> data;
		};


		static std::shared_ptr<CommandBuffer> Create();
		//resets all operations
		void Reset();
		void BindRenderPass(std::shared_ptr<RenderPass>& renderPass, std::shared_ptr<Framebuffer>& framebuffer, glm::vec4 clearColor);
		void BindGraphicsPipeline(std::shared_ptr<RoxEngine::GraphicsPipeline>& pipeline);
		void BindVertexArray(std::shared_ptr<RoxEngine::VertexArray>& va);
		void Draw(uint32_t count);
		void BlitFramebuffers(std::shared_ptr<RoxEngine::Framebuffer> src, std::shared_ptr<RoxEngine::Framebuffer> dst);
		void CallCmd(std::shared_ptr<CommandBuffer> cmd);
		void InlineCmd(std::shared_ptr<CommandBuffer> cmd);
		void RawCall(std::function<void(CommandBuffer*,void*)> fn);
		void UnbindRenderPass();
		void UnbindGraphicsPipeline();
		void UnbindVertexArray();

		// creates the raw objs without executing
		bool HasChanged();
		virtual void CreateCache() = 0;
		virtual void Execute() = 0;
	protected:
		bool mChanged = false;
		std::vector<Operation> mOperations;
	};
}

