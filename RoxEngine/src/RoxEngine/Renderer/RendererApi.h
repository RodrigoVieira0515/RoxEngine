#pragma once
#include <memory>
#include <RoxEngine/Renderer/GraphicsContext.h>
#include <RoxEngine/Renderer/Buffer.h>
#include <RoxEngine/Renderer/VertexArray.h>

namespace RoxEngine {
    enum class API {
        NONE,
        OPENGL,
    };
    class RendererApi {
    public:
        static void Init(std::shared_ptr<GraphicsContext> context, API api = API::NONE); // Acts as initialize
        static std::shared_ptr<RendererApi> Get();
        static void Shutdown();
    private:
        friend class VertexArray;
        friend class VertexBuffer;
        friend class IndexBuffer;

        virtual std::shared_ptr<VertexBuffer> CreateVB(void* vertices, size_t size) = 0;
        virtual std::shared_ptr<IndexBuffer> CreateIB(uint32_t* indices, size_t size) = 0;
        virtual std::shared_ptr<VertexArray> CreateVA() = 0;

        std::shared_ptr<GraphicsContext> mContext;
    };
}