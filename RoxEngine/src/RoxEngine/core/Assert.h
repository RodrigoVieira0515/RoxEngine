#pragma once
#define RE_ENABLE_ASSERTS
#include <RoxEngine/core/PlatformDetection.h>
#include <RoxEngine/core/Logger.h>

#if defined(RE_PLATFORM_WINDOWS)
#define RE_DEBUGBREAK() __debugbreak()
#elif defined(RE_PLATFORM_LINUX)
#include <signal.h>
#define RE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif

#ifdef RE_ENABLE_ASSERTS
#define RE_ASSERT(x, ...) { if(!(x)) { RE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }
#define RE_CORE_ASSERT(x, ...) { if(!(x)) { RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }
#else
#define RE_ASSERT(x, ...)
#define RE_CORE_ASSERT(x, ...)
#endif

#define RE_VERIFY(x, ...) { if(!(x)) { RE_ERROR("Verification Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }
#define RE_CORE_VERIFY(x, ...) { if(!(x)) { RE_CORE_ERROR("Verification Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }