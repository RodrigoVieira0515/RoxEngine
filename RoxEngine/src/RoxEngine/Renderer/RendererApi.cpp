#include "RendererApi.h"
#include "RendererApi_private.h"
#include "RoxEngine/Renderer/GraphicsContext.h"
#include <RoxEngine/Core/Logger.h>
#include <Platforms/Renderer/DefaultGLRendererApi.h>
#include <memory>

namespace RoxEngine {
    API RendererApiData::sAPI = API::NONE;
    std::shared_ptr<RendererApi> RendererApiData::sRendererAPI = nullptr;

    void RendererApi::Init(std::shared_ptr<GraphicsContext> context, API api) {
        if(api == API::NONE) {
            api = API::OPENGL; // auto choose
        }
        switch(api) {
        case API::OPENGL:
            RendererApiData::sRendererAPI = std::make_shared<Platforms::Renderer::DefaultGLRendererAPI>();
            break;
        default: {
            CORE_CRITICAL("Unknown RendererAPI!");
            std::exit(3);
        }

        }   
    }
    std::shared_ptr<RendererApi> RendererApi::Get() {
        return RendererApiData::sRendererAPI;
    }
    void RendererApi::Shutdown() {

    }
}