#include "tagepch.h"
#include "Context.h"

namespace TAGE::RENDERER {
	void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		CORE_LOG_ERROR("[OpenGL Debug] {0}", message);
	}
	void Context::Init()
	{
		glfwMakeContextCurrent(_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(status, "Failed to initialize GLAD!");

		CORE_LOG_INFO("OpenGL Info:");
		CORE_LOG_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		CORE_LOG_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		CORE_LOG_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST);
		glEnable(GL_MULTISAMPLE);

#if LOG_GRAPICHS_ERROR
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
	}
}