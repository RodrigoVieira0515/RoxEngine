#include "Window.h"
#include "Platforms/Window/GlfwWindow.h"
#include <memory>
namespace RoxEngine {
    std::shared_ptr<Window> Window::Create(const Desc &desc) {
        #ifdef ROXENGINE_INCLUDE_GLFW
            return std::make_shared<Platforms::Window::Glfw>(desc);
        #endif
        return nullptr;
    }
}