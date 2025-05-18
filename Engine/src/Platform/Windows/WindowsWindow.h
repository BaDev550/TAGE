#pragma once

#include "TAGE/Window/Window.h"
#include "TAGE/Renderer/Render/Context.h"
#include "GLFW/glfw3.h"

namespace TAGE {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const SWindowPropeties& props);
		virtual ~WindowsWindow();

		int GetWidth() const override { return _Data.Width; }
		int GetHeight() const override { return _Data.Height; }
		virtual bool ShoudClose() const override;

		virtual void SwapBuffers() const override;
		virtual void EnableCursor(bool enable) const override;
		virtual void IconifyWindow() override;
		virtual bool IsWindowIconifyed() override;
		virtual void RestoreWindow(bool maximized) override;
		virtual void SetWindowPos(int x,  int y) override;
		virtual void GetWindowPos(int* x, int* y) override;
		virtual void SetWindowIcon(const std::string& iconPath) override;

		void SetEventCallback(const EventCallbackFn& callback) override { _Data.EventCallback = callback; }
		void* GetNativeWindow() const override { return static_cast<void*>(_Window); }
	private:
		virtual void Init(const SWindowPropeties& props);
		virtual void Shutdown();

		struct WindowData
		{
			std::string Title;
			int Width, Height;
			EventCallbackFn EventCallback;
		};

		GLFWwindow* _Window;
		MEM::Ref<RENDERER::Context> _Context;
	public:
		WindowData _Data;
	};
}