#include <RoxEngine/Renderer/Buffer.h>

namespace RoxEngine::Platforms::Renderer {
    class DefaultGLVertexBuffer final : public ::RoxEngine::VertexBuffer {
    public:
        DefaultGLVertexBuffer(void* vertices, size_t size);
        virtual ~DefaultGLVertexBuffer() final;
        virtual void SetData(const void* data, size_t size) override final;
        virtual const BufferLayout& GetLayout() const override final;
        virtual void SetLayout(const BufferLayout& layout) override final;

        uint32_t mID;
        size_t mSize;
        BufferLayout mLayout;
    };
    class DefaultGLIndexBuffer final : public ::RoxEngine::IndexBuffer {
    public:
        DefaultGLIndexBuffer(uint32_t* vertices, size_t count);
        virtual ~DefaultGLIndexBuffer() final;
        virtual void SetData(const uint32_t* data, size_t count) override final;
        virtual size_t GetCount() const final;

        uint32_t mID;
        size_t mCount;
        BufferLayout mLayout;
    };
}