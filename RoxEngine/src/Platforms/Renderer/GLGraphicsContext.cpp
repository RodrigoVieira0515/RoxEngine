#include <glad/gl.h>
#include "GLGraphicsContext.h"
#include "RoxEngine/Core/Logger.h"
namespace RoxEngine::Platforms::Renderer {
    GLGraphicsContext::GLGraphicsContext(GLFWwindow* window) {
        mWindow = window;
    }
    void GLGraphicsContext::Init() {
        glfwMakeContextCurrent(mWindow);
		int status = gladLoadGL(glfwGetProcAddress);
		if(!status) {
            CORE_CRITICAL("ERROR: Failed to initialize Glad!");
            std::exit(2);
        }

		CORE_INFO("OpenGL Info:");
		CORE_INFO("  Vendor: {0}",   (const char*)glGetString(GL_VENDOR));
		CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		CORE_INFO("  Version: {0}",  (const char*)glGetString(GL_VERSION));
    }
    void GLGraphicsContext::SwapBuffers() {
        glfwSwapBuffers(mWindow);
    }
}