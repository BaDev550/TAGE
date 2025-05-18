#include "tagepch.h"
#include "WindowsWindow.h"

#include "TAGE/Events/Event.h"
#include "TAGE/Events/ApplicationEvents.h"
#include "TAGE/Events/InputEvents.h"

#include "stb/stb_image.h"

namespace TAGE {
	static bool s_WindowClassRegistered = false;
	static void GLFWErrorCallback(int err, const char* desc);

	WindowsWindow::WindowsWindow(const SWindowPropeties& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const SWindowPropeties& props)
	{
		_Data.Title = props.Title;
		_Data.Width = props.Width;
		_Data.Height = props.Height;

		CORE_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_WindowClassRegistered) {
			int success = glfwInit();
			ENGINE_ASSERT(success, "Failed to initialize GLFW");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			glfwWindowHint(GLFW_SAMPLES, 4);
			glfwSetErrorCallback(GLFWErrorCallback);
			s_WindowClassRegistered = true;
		}

		_Window = glfwCreateWindow(_Data.Width, _Data.Height, _Data.Title.c_str(), NULL, NULL);
		ENGINE_ASSERT(_Window != NULL, "Failed to create GLFW window");
		glfwSetWindowUserPointer(_Window, &_Data);
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		if (primaryMonitor) {
			GLFWvidmode const* mode = glfwGetVideoMode(primaryMonitor);
			if (mode) {
				int monitorX, monitorY;
				glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

				int windowX = monitorX + (mode->width - _Data.Width) / 2;
				int windowY = monitorY + (mode->height - _Data.Height) / 2;

				glfwSetWindowPos(_Window, windowX, windowY);
			}
		}

		_Context = MEM::CreateRef<RENDERER::Context>((GLFWwindow*)_Window);
		_Context->Init();
		glfwSwapInterval(1);

		glfwSetWindowSizeCallback(_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS: {
				KeyPressedEvent event((KeyCode)key);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event((KeyCode)key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event((KeyCode)key);
				data.EventCallback(event);
				break;
			}
			default:
				std::cout << "Incorrect Key action!" << std::endl;
				break;
			}
			});

		glfwSetMouseButtonCallback(_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS: {
				MouseButtonPressedEvent event((MouseButton)button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event((MouseButton)button);
				data.EventCallback(event);
				break;
			}
			default:
				std::cout << "Incorrect Mouse action!" << std::endl;
				break;
			}
			});

		glfwSetScrollCallback(_Window, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)x, (float)y);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(_Window);
		glfwTerminate();
	}

	bool WindowsWindow::ShoudClose() const
	{
		return glfwWindowShouldClose(_Window);
	}

	void WindowsWindow::SwapBuffers() const
	{
		glfwPollEvents();
		_Context->SwapBuffers();
	}

	void WindowsWindow::EnableCursor(bool enable) const
	{
		glfwSetInputMode(_Window, GLFW_CURSOR, enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	void WindowsWindow::IconifyWindow()
	{
		glfwIconifyWindow(_Window);
	}

	bool WindowsWindow::IsWindowIconifyed()
	{
		return glfwGetWindowAttrib(_Window, GLFW_ICONIFIED);
	}

	void WindowsWindow::RestoreWindow(bool maximized)
	{
		if (maximized) {
			glfwRestoreWindow(_Window);
		}
		else {
			glfwMaximizeWindow(_Window);
		}
	}

	void WindowsWindow::SetWindowPos(int x, int y)
	{
		glfwSetWindowPos(_Window, x, y);
	}

	void WindowsWindow::GetWindowPos(int* x, int* y)
	{
		glfwGetWindowPos(_Window, x, y);
	}

	void WindowsWindow::SetWindowIcon(const std::string& iconPath)
	{
		int width, height, channels;
		unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4); // 4 = RGBA

		if (!pixels)
		{
			CORE_LOG_ERROR("Failed to load window icon: {0}", iconPath);
			return;
		}

		GLFWimage icon;
		icon.width = width;
		icon.height = height;
		icon.pixels = pixels;
		glfwSetWindowIcon(_Window, 1, &icon);

		stbi_image_free(pixels);
	}

	void GLFWErrorCallback(int err, const char* desc)
	{
		CORE_LOG_ERROR("GLFW Error [{0}], {1}", err, desc);
	}
}