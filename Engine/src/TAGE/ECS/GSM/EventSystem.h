#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include "TAGE/Events/Event.h"

namespace TAGE {

    class EventSystem {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        template<typename T>
        void AddListener(std::function<void(T&)> listener) {
            auto type = std::type_index(typeid(T));
            auto wrapper = [listener](Event& e) {
                listener(static_cast<T&>(e));
                };
            _listeners[type].push_back(wrapper);
        }

        void Dispatch(Event& event) {
            auto type = std::type_index(typeid(event));
            if (_listeners.count(type)) {
                for (auto& listener : _listeners[type]) {
                    listener(event);
                    if (event.bIsHandled)
                        break;
                }
            }
        }

    private:
        std::unordered_map<std::type_index, std::vector<EventCallbackFn>> _listeners;
    };

}
