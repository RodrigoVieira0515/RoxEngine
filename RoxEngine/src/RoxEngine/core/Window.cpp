#include "Window.h"
#include <RoxEngine/core/Logger.h>
#include <RoxEngine/core/Assert.h>
#include <RoxEngine/renderer/RendererApi.h>
#include <RoxEngine/core/PlatformDetection.h>

#ifdef RE_PLATFORM_WINDOWS
#include <Platform/Windows/WWindow.h>
#endif

namespace RoxEngine {
    std::shared_ptr<Window> Window::Create(const WindowDesc& desc)
    {
        #ifdef RE_PLATFORM_WINDOWS
            return std::make_shared<Windows::Window>(desc);
        #endif
    }
}