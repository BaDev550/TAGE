#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include "TAGE/Core/Utilities/Memory.h"

namespace TAGE {
	class ServiceLocator
	{
    public:
        template<typename T>
        static void RegisterService(MEM::Ref<T> service) {
            auto typeId = std::type_index(typeid(T));
            _services[typeId] = service;
        }

        template<typename T>
        static MEM::Ref<T> GetService() {
            auto typeId = std::type_index(typeid(T));
            auto it = _services.find(typeId);
            if (it != _services.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            throw std::runtime_error("Service not registered.");
        }

        static void Clear() {
            _services.clear();
        }
    private:
        static std::unordered_map<std::type_index, MEM::Ref<void>> _services;
	};
}