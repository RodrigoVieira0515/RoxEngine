#include "Buffer.h"
#include "RendererApi_private.h"
#include <memory>

namespace RoxEngine {
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
        return RendererApiData::sRendererAPI->CreateVB(nullptr, size);
    }
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(void* vertices, size_t size) {
        return RendererApiData::sRendererAPI->CreateVB(vertices, size);
    }
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, size_t count) {
        return RendererApiData::sRendererAPI->CreateIB(indices, count);
    }
}