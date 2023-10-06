#pragma once
#include <memory>
#include <functional>
#include <RoxEngine/events/Event.h>

namespace RoxEngine {
	struct WindowDesc {
		const char* name;
		uint32_t width,height;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void* GetNativeWindow() = 0;
		virtual bool IsOpen() = 0;
		virtual void PollEvents() = 0;
		virtual WindowDesc GetDesc() = 0;
		virtual std::array<uint32_t, 2> GetSize() = 0;
		virtual void SetEventCallback(EventCallbackFn fn) = 0;

		static std::shared_ptr<Window> Create(const WindowDesc& desc);
	};
}
