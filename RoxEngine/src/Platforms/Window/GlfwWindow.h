#pragma once
#include <RoxEngine/Core/Window.h>
#include <GLFW/glfw3.h>

namespace RoxEngine::Platforms::Window {
    class Glfw final : public ::RoxEngine::Window{
    public:
        Glfw(const Desc& desc);
        virtual ~Glfw() final;
        virtual bool IsOpen() final;
        virtual void PollEvents() final;
        virtual void* GetNative() final;

        GLFWwindow* mWindow;
        Desc mDesc;
    };
}