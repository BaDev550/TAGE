#pragma once

#include <unordered_map>
#include <string>
#include "TAGE/Events/Input.h"
#include "TAGE/Core/Utilities/Memory.h"

namespace TAGE::ECS {

    enum class InputActionState {
        None,
        Pressed,
        Released,
        Held
    };

    struct InputAction {
        KeyCode Key;
        InputActionState State = InputActionState::None;
        bool WasPressedLastFrame = false;
    };

    class InputComponent {
    public:
        void BindAction(const std::string& name, KeyCode key) {
            _Actions[name] = InputAction{ key };
        }

        InputActionState GetActionState(const std::string& name) {
            auto it = _Actions.find(name);
            if (it == _Actions.end()) return InputActionState::None;
            return it->second.State;
        }

        void Update() {
            for (auto& [name, action] : _Actions) {
                bool isPressed = Input::IsKeyPressed(action.Key);
                if (isPressed && !action.WasPressedLastFrame)
                    action.State = InputActionState::Pressed;
                else if (!isPressed && action.WasPressedLastFrame)
                    action.State = InputActionState::Released;
                else if (isPressed)
                    action.State = InputActionState::Held;
                else
                    action.State = InputActionState::None;

                action.WasPressedLastFrame = isPressed;
            }
        }

    private:
        std::unordered_map<std::string, InputAction> _Actions;
    };

}
