#include "VertexArray.h"
#include "RendererApi_private.h"

namespace RoxEngine {
    std::shared_ptr<VertexArray> VertexArray::Create() {
        return RendererApiData::sRendererAPI->CreateVA();
    }
}