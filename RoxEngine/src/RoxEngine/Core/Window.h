#pragma once
#include <memory>
#include <string>

namespace RoxEngine {
    class Window {
    public:
        struct Desc {
            std::string title;
            uint32_t width,height;
        };
        static std::shared_ptr<Window> Create(const Desc& desc);
        virtual bool IsOpen() = 0;
        virtual void PollEvents() = 0;
        virtual void* GetNative() = 0;
        virtual ~Window() = default;
    };
}