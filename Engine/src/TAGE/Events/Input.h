#pragma once

#include "TAGE/Core/Utilities/PlatformDetection.h"
#include <glm/glm.hpp>

#if defined(PLATFORM_WINDOWS)
    #include "Platform/Windows/WindowsKeyCodes.h"
    #include "Platform/Windows/WindowsMouseCodes.h"
#elif defined(PLATFORM_LINUX)
    #include "Platform/Linux/LinuxKeyCodes.h"
#elif defined(PLATFORM_MAC)
    #include "Platform/Mac/MacKeyCodes.h"
#endif

namespace TAGE {

    enum class KeyCode {
        A = PE_KEY_A,
        B = PE_KEY_B,
        C = PE_KEY_C,
		D = PE_KEY_D,
		E = PE_KEY_E,
		F = PE_KEY_F,
		G = PE_KEY_G,
		H = PE_KEY_H,
		I = PE_KEY_I,
		J = PE_KEY_J,
		K = PE_KEY_K,
		L = PE_KEY_L,
		M = PE_KEY_M,
		N = PE_KEY_N,
		O = PE_KEY_O,
		P = PE_KEY_P,
		Q = PE_KEY_Q,
		R = PE_KEY_R,
		S = PE_KEY_S,
		T = PE_KEY_T,
		U = PE_KEY_U,
		V = PE_KEY_V,
		W = PE_KEY_W,
		X = PE_KEY_X,
		Y = PE_KEY_Y,
		Z = PE_KEY_Z,
		Num0 = PE_KEY_0,
		Num1 = PE_KEY_1,
		Num2 = PE_KEY_2,
		Num3 = PE_KEY_3,
		Num4 = PE_KEY_4,
		Num5 = PE_KEY_5,
		Num6 = PE_KEY_6,
		Num7 = PE_KEY_7,
		Num8 = PE_KEY_8,
		Num9 = PE_KEY_9,
		Left = PE_KEY_LEFT,
		Right = PE_KEY_RIGHT,
		Up = PE_KEY_UP,
		Down = PE_KEY_DOWN,
		LShift = PE_KEY_LEFT_SHIFT,
		LControl = PE_KEY_LEFT_CONTROL,
		LAlt = PE_KEY_LEFT_ALT,
		RShift = PE_KEY_RIGHT_SHIFT,
		RControl = PE_KEY_RIGHT_CONTROL,
		RAlt = PE_KEY_RIGHT_ALT,
		Escape = PE_KEY_ESCAPE,
		F1 = PE_KEY_F1,
		F2 = PE_KEY_F2,
		F3 = PE_KEY_F3,
		F4 = PE_KEY_F4,
		F5 = PE_KEY_F5,
		F6 = PE_KEY_F6,
		F7 = PE_KEY_F7,
		F8 = PE_KEY_F8,
		F9 = PE_KEY_F9,
		Space = PE_KEY_SPACE,
		Enter = PE_KEY_ENTER,
		Tab = PE_KEY_TAB,
    };

    enum class MouseButton {
        Left = PE_MOUSE_BUTTON_LEFT,
        Right = PE_MOUSE_BUTTON_RIGHT,
        Middle = PE_MOUSE_BUTTON_MIDDLE
    };

	inline bool operator==(KeyCode lhs, int rhs) {
		return static_cast<int>(lhs) == rhs;
	}

	inline bool operator==(int lhs, KeyCode rhs) {
		return lhs == static_cast<int>(rhs);
	}

    class Input {
    public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseButton button);
		static glm::vec2 GetMousePosition();
    };
}