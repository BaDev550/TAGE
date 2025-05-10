#include "tagepch.h"
#include "Input.h"
#include "GLFW/glfw3.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE {
    static GLFWwindow* GetGLFWWindow() {
        return (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    }

    bool Input::IsKeyPressed(KeyCode key) {
        auto window = GetGLFWWindow();
        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseButton button) {
        auto window = GetGLFWWindow();
        int state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition() {
        double xpos, ypos;
        glfwGetCursorPos(GetGLFWWindow(), &xpos, &ypos);
        return { (float)xpos, (float)ypos };
    }
}