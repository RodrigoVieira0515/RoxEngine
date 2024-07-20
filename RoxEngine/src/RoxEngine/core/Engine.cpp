#include "Engine.hpp"
#include "RoxEngine/imgui/imgui.hpp"
#include "RoxEngine/renderer/GraphicsContext.hpp"
#include "imgui.h"
#include <RoxEngine/profiler/profiler.hpp>
#include <GLFW/glfw3.h>
#include <RoxEngine/platforms/GLFW/GLFWWindow.hpp>
#include <RoxEngine/input/Input.hpp>
#include <iostream>
#include "RoxEngine/renderer/Shader.hpp"

namespace RoxEngine {
    static Engine* sEngine = nullptr;
    Engine* Engine::Get() {
        if(!sEngine)
            sEngine = new Engine();
        return sEngine;
    }
    namespace FileSystem
    {
        extern std::string resourcesFolder;
    }
    int Engine::Run(Scope<Game> game) {
        //TODO: add the option to change the folder at initialization not runtime
        FileSystem::resourcesFolder = "./resources";
        PROFILER_BEGIN_SESSION("RoxEngine");
        if(!glfwInit()) return 1;
        mWindow = CreateRef<GLFW::Window>();
        GraphicsContext::Init(RendererApi::OPENGL);
        GraphicsContext::ClearColor(100.0f/255.0f,149.0f/255.0f,237.0f/255.0f);
        ImGuiLayer::Init();
        Input::Init();
        game->Init();
        while(mWindow->IsOpen()) {
            PROFILER_SCOPE("Frame");
            Input::Update();
            mWindow->PollEvents();
            game->Update();
            ImGuiLayer::NewFrame();
            game->Render();
            DrawDebugInfo();
            ImGuiLayer::Render();
            glfwSwapBuffers(static_cast<GLFWwindow*>(std::static_pointer_cast<GLFW::Window>(mWindow)->mWindow));
        }
        game.reset();
        Input::Shutdown();
        ImGuiLayer::Shutdown();
        GraphicsContext::Shutdown();
        glfwTerminate();
        delete sEngine;
        PROFILER_END_SESSION();
        return 0; // OK
    }
    void Engine::DrawDebugInfo() {
        mWindow = mWindow;
    }
}