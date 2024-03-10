#include "GlfwWindow.h"
#include "GLFW/glfw3.h"
#include "RoxEngine/Core/Logger.h"

using namespace RoxEngine;

void error_callback(int code, const char* description)
{
    if(code == GLFW_NOT_INITIALIZED) {
        CORE_CRITICAL("GLFW ERROR({}): {}!", code, description);
        std::exit(1);
    }
    CORE_ERROR("GLFW ERROR({}): {}", code, description);
}


namespace RoxEngine::Platforms::Window {
    Glfw::Glfw(const Desc& desc) : mDesc(desc) {
        if(!glfwInit()) {
            CORE_CRITICAL("GLFW ERROR: Was not able to initialize!");
            std::exit(1);
        }
        glfwSetErrorCallback(error_callback);

        mWindow = glfwCreateWindow(mDesc.width, mDesc.height, mDesc.title.c_str(), nullptr, nullptr);
    }
    Glfw::~Glfw() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
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