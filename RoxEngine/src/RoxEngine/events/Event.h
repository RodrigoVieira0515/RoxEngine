#pragma once
#include <string>
#include <sstream>
#include <RoxEngine/core/Keycodes.h>
#include <RoxEngine/core/Mousecodes.h>
#include <spdlog/fmt/bundled/format.h>

namespace RoxEngine {

enum class EventType : uint32_t
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
};

enum class EventCategory : uint32_t
{
	None = 0,
	Application = 1 << 0,
	Input = 1 << 1,
	Keyboard = 1 << 2,
	Mouse = 1 << 3,
	MouseButton = 1 << 4,
};
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual uint32_t GetCategoryFlags() const override { return (uint32_t)(category); }

class Window;

class Event
{
public:
	virtual ~Event() = default;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual uint32_t GetCategoryFlags() const = 0;
	virtual std::string ToString() const {return GetName(); }

	bool IsInCategory(EventCategory category)
	{
		return GetCategoryFlags() & (uint32_t)category;
	}
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{
	}

	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};
class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(unsigned int width, unsigned int height)
		: mWidth(width), mHeight(height) {
	}

	unsigned int GetWidth() const { return mWidth; }
	unsigned int GetHeight() const { return mHeight; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::WindowResize)
	EVENT_CLASS_CATEGORY(EventCategory::Application)


	Window* window;
private:
	unsigned int mWidth, mHeight;

};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {
	}

	Window* window;

	EVENT_CLASS_TYPE(EventType::WindowClose)
	EVENT_CLASS_CATEGORY(EventCategory::Application)
};

class AppTickEvent : public Event
{
public:
	AppTickEvent() {};

	EVENT_CLASS_TYPE(EventType::AppTick)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {};

	EVENT_CLASS_TYPE(EventType::AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}
	EVENT_CLASS_TYPE(EventType::AppRender)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
};

class KeyEvent : public Event
{
public:
	KeyCode GetKeyCode() {}

	EVENT_CLASS_CATEGORY((uint32_t)EventCategory::Keyboard | (uint32_t)EventCategory::Input);

protected:
	KeyEvent(KeyCode keycode)
		: mKeyCode(keycode) {
	}

	KeyCode mKeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
		: KeyEvent(keycode), mIsRepeat(isRepeat) {
	}

	bool IsRepeat() const { return mIsRepeat; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << mKeyCode << " (repeat = " << mIsRepeat << ")";
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KeyPressed)
private:
	bool mIsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyCode keycode)
		: KeyEvent(keycode) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << mKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const KeyCode keycode)
		: KeyEvent(keycode) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << mKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::KeyTyped)
};
class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, const float y)
		: mMouseX(x), mMouseY(y) {
	}

	float GetX() { return mMouseX; }
	float GetY() { return mMouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MouseMoved)
		EVENT_CLASS_CATEGORY((uint32_t)EventCategory::Mouse | (uint32_t)EventCategory::Input)
private:
	float mMouseX, mMouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
		: mXOffset(xOffset), mYOffset(yOffset) {
	}

	float GetXOffset() const {return mXOffset; }
	float GetYOffset() const {return mYOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MouseScrolled)
		EVENT_CLASS_CATEGORY((uint32_t)EventCategory::Mouse | (uint32_t)EventCategory::Input)
private:
	float mXOffset, mYOffset;
};

class MouseButtonEvent : public Event
{
public:
	MouseCode GetMouseButton() const { return m_Button; }

	EVENT_CLASS_CATEGORY((uint32_t)EventCategory::Mouse | (uint32_t)EventCategory::Input | (uint32_t)EventCategory::MouseButton)
protected:
	MouseButtonEvent(const MouseCode button)
		: m_Button(button) {
	}

	MouseCode m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button)
		: MouseButtonEvent(button) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseCode button)
		: MouseButtonEvent(button) {
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
};
}