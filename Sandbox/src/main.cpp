#include <iostream>
#include <RoxEngine/Core/Logger.h>
#include <RoxEngine/Core/Thread.h>
#include <RoxEngine/Core/Window.h>
#include <RoxEngine/Renderer/GraphicsContext.h>

using namespace RoxEngine;

int main(){
    SetThreadName("GameThread");
    Window::Desc window_desc;
    window_desc.title = "RoxEngine - SANDBOX";
    window_desc.width = 800;
    window_desc.height = 800;
    auto window = Window::Create(window_desc);
    auto context = GraphicsContext::Create(window->GetNative());
    context->Init();
    while(window->IsOpen()) {
        window->PollEvents();

        context->SwapBuffers();
    }
}