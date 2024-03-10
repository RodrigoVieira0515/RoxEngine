#pragma once
#include "GLFW/glfw3.h"
#include <RoxEngine/Renderer/GraphicsContext.h>
namespace RoxEngine::Platforms::Renderer {
    class GLGraphicsContext final : public ::RoxEngine::GraphicsContext {
    public:
        GLGraphicsContext(GLFWwindow* window);
        virtual void Init() final;
        virtual void SwapBuffers() final;

        GLFWwindow* mWindow;
    };
}