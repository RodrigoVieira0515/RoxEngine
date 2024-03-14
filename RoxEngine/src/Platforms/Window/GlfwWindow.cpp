#include "GlfwWindow.h"
#include "GLFW/glfw3.h"
#include "RoxEngine/Core/Logger.h"

using namespace RoxEngine;

namespace RoxEngine::Platforms::Window {
    Glfw::Glfw(const Desc& desc) : mDesc(desc) {
        mWindow = glfwCreateWindow(mDesc.width, mDesc.height, mDesc.title.c_str(), nullptr, nullptr);
    }
    Glfw::~Glfw() {
        glfwDestroyWindow(mWindow);
    }
    bool Glfw::IsOpen() {
        return !glfwWindowShouldClose(mWindow);
    }
    void Glfw::PollEvents() {
        glfwPollEvents();
    }
    void* Glfw::GetNative() {
        return mWindow;
    }
}