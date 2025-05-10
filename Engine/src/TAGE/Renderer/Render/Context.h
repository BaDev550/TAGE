#pragma once

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#include "TAGE/Core/Defines.h"
#include "TAGE/Core/Utilities/Logger.h"
namespace TAGE::RENDERER {
	class Context {
	public:
		Context(GLFWwindow* windowHandle) : _WindowHandle(windowHandle) {}
		void Init();

		void SwapBuffers()
		{
			glfwSwapBuffers(_WindowHandle);
		}
	private:
		GLFWwindow* _WindowHandle;
	};
}