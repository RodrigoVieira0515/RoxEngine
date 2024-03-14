#pragma once

#include "RoxEngine/Renderer/Buffer.h"
#include "RoxEngine/Renderer/RendererApi.h"
namespace RoxEngine::Platforms::Renderer {
    class DefaultGLRendererAPI final : public ::RoxEngine::RendererApi{
    public:
        DefaultGLRendererAPI() = default;
        void clearcolor(float r,float g,float b,float a=1.0f);
        void clearscreen();

        virtual ~DefaultGLRendererAPI() final = default;
        virtual std::shared_ptr<VertexBuffer> CreateVB(void* vertices, size_t size) final;
        virtual std::shared_ptr<IndexBuffer> CreateIB(uint32_t* indices, size_t count) final;
        virtual std::shared_ptr<VertexArray> CreateVA() final;
    };
}