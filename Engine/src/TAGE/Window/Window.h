#pragma once

#include "TAGE/Core/Utilities/PlatformDetection.h"
#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Events/Event.h"
#include "TAGE/Core/Utilities/Logger.h"

namespace TAGE {
	struct SWindowPropeties {
		std::string Title;
		int Width;
		int Height;
		SWindowPropeties(const std::string& title = "TAGE", int width = 1280, int height = 720) : Title(title), Height(height), Width(width) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		virtual ~Window() = default;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual bool ShoudClose() const = 0;
		virtual void EnableCursor(bool enable) const = 0;
		virtual void IconifyWindow() = 0;
		virtual bool IsWindowIconifyed() = 0;
		virtual void RestoreWindow(bool maximized) = 0;
		virtual void SetWindowPos(int x,  int y) = 0;
		virtual void GetWindowPos(int* x, int* y) = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SwapBuffers() const = 0;
		virtual void* GetNativeWindow() const = 0;

		static MEM::Scope<Window> Create(const SWindowPropeties& props = SWindowPropeties());
	};
}