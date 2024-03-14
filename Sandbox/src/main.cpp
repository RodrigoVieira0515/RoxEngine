#include "RoxEngine/Renderer/Buffer.h"
#include "RoxEngine/Renderer/RendererApi.h"
#include "RoxEngine/Renderer/VertexArray.h"
#include <RoxEngine/Core/Logger.h>
#include <RoxEngine/Core/Thread.h>
#include <RoxEngine/Core/Window.h>
#include <RoxEngine/Renderer/GraphicsContext.h>
#include <RoxEngine/Core/Platform.h>
#include <Platforms/Renderer/DefaultGLRendererApi.h>
#include <Platforms/Renderer/DefaultGLVertexArray.h>
#include <Platforms/Renderer/DefaultGLBuffer.h>
#include <glad/gl.h>
#include <memory>
using namespace RoxEngine;

int main(){
    SetThreadName("GameThread");
    Platform::Init();
    {
        Window::Desc window_desc;
        window_desc.title = "RoxEngine - SANDBOX";
        window_desc.width = 800;
        window_desc.height = 800;
        auto window = Window::Create(window_desc);
        
        auto context = GraphicsContext::Create(window->GetNative());
        context->Init();
        RendererApi::Init(context);
        auto api = std::dynamic_pointer_cast<Platforms::Renderer::DefaultGLRendererAPI>(RendererApi::Get());
        api->clearcolor(100./255., 149./255., 237./255.);
        
        auto va = VertexArray::Create();
        {
            float vertices[] = {
                -.5,-.5,.0,
                -.5, .5,.0,
                .5, .5,.0,
                .5,-.5,.0,
            };
            uint32_t indices[] = {
                0,2,1,
                0,3,2
            };

            auto vb = VertexBuffer::Create(vertices, sizeof(float)*3*4);
            vb->SetLayout({{"mPos", ShaderDataType::Float3}});
            auto ib = IndexBuffer::Create(indices, 6);

            va->AddVertexBuffer(vb);
            va->SetIndexBuffer(ib);
        }

        while(window->IsOpen()) {
            window->PollEvents();
            api->clearscreen();

            {
                auto glva = std::static_pointer_cast<Platforms::Renderer::DefaultGLVertexArray>(va);
                auto glib = std::static_pointer_cast<Platforms::Renderer::DefaultGLIndexBuffer>(va->GetIndexBuffer());

                glBindVertexArray(glva->mID);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glib->mID);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }

            context->SwapBuffers();
        }
        RendererApi::Shutdown();
    }
    Platform::Shutdown();
}