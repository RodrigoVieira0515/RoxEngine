#include "DefaultGLRendererApi.h"
#include "RoxEngine/Renderer/Buffer.h"
#include "DefaultGLBuffer.h"
#include "DefaultGLVertexArray.h"
#include <memory>
#include <glad/gl.h>

namespace RoxEngine::Platforms::Renderer {
    void DefaultGLRendererAPI::clearcolor(float r, float g,float b, float a) {
        glClearColor(r,g,b,a);
    }
    void DefaultGLRendererAPI::clearscreen() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    std::shared_ptr<VertexBuffer> DefaultGLRendererAPI::CreateVB(void* vertices, size_t size) {
        return std::make_shared<DefaultGLVertexBuffer>(vertices, size);
    }
    std::shared_ptr<IndexBuffer> DefaultGLRendererAPI::CreateIB(uint32_t* indices, size_t count) {
        return std::make_shared<DefaultGLIndexBuffer>(indices, count);
    }
    std::shared_ptr<VertexArray> DefaultGLRendererAPI::CreateVA() {
        return std::make_shared<DefaultGLVertexArray>();
    }
}