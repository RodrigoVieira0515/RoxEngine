#include "GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "Platforms/Renderer/GLGraphicsContext.h"
#include <memory>
namespace RoxEngine {
    std::shared_ptr<GraphicsContext> GraphicsContext::Create(void *window) {
        #ifdef ROXENGINE_INCLUDE_OPENGL
            return std::make_shared<Platforms::Renderer::GLGraphicsContext>(static_cast<GLFWwindow*>(window));
        #endif

        return nullptr;
    }
}