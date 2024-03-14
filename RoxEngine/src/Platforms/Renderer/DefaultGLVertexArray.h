#include "RoxEngine/Renderer/Buffer.h"
#include <RoxEngine/Renderer/VertexArray.h>
#include <memory>

namespace RoxEngine::Platforms::Renderer {
    class DefaultGLVertexArray final : public ::RoxEngine::VertexArray {
    public:
        DefaultGLVertexArray();
        virtual ~DefaultGLVertexArray() final;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override final;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override final;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const final;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const final;

        uint32_t mID = 0, mVBIndex = 0;
        std::vector<std::shared_ptr<VertexBuffer>> mVBs;
        std::shared_ptr<IndexBuffer> mIB;
    };
}