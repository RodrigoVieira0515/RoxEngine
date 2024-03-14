#include "Platform.h"
#include "Logger.h"
#include <GLFW/glfw3.h>

namespace RoxEngine {
    void error_callback(int code, const char* description)
    {
        if(code == GLFW_NOT_INITIALIZED) {
            CORE_CRITICAL("GLFW ERROR({}): {}!", code, description);
            std::exit(1);
        }
        CORE_ERROR("GLFW ERROR({}): {}", code, description);
    }

    void Platform::Init() {
        if(!glfwInit()) {
            CORE_CRITICAL("GLFW ERROR: Was not able to initialize!");
            std::exit(1);
        }
        glfwSetErrorCallback(error_callback);
    }
    void Platform::Shutdown() {
        glfwTerminate();
    }
}