#pragma once

#include <memory>
namespace RoxEngine {
    class GraphicsContext {
    public:
        static std::shared_ptr<GraphicsContext> Create(void* window);
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}