#pragma once

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define PLATFORM_MACOS
#elif defined(__linux__)
#define PLATFORM_LINUX
#else
#error "Unknown Platform"
#endif

namespace Platform {
	enum class EPlatform {
		WINDOWS,
		LINUX,
		MAC,
		UNDEFINED
	};

	inline constexpr EPlatform DetectPlatform() {
#if defined(PLATFORM_WINDOWS)
		return EPlatform::WINDOWS;
#elif defined(PLATFORM_LINUX)
		return EPlatform::LINUX
#elif defined(PLATFORM_MAC)
		return EPlatform::MAC
#else
		return EPlatform::UNDEFINED
#endif
	}

	inline EPlatform GetPlatform() {
		static const EPlatform platform = DetectPlatform();
		return platform;
	}
}