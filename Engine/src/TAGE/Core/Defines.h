#pragma once
#include "TAGE/Core/Utilities/Logger.h"
#include "TAGE/ECS/Objects/GameObjectFactory.h"

#define ENGINE_ASSERT(x, ...) if (!(x)) { CORE_LOG_ERROR("Assertion failed: {}", __VA_ARGS__); __debugbreak(); }
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define LOG_GRAPICHS_ERROR 0

#define REGISTER_GAMEOBJECT_CLASS(CLASS) \
    static struct CLASS##FactoryRegister { \
        CLASS##FactoryRegister() { \
            TAGE::ECS::GameObjectFactory::Register(#CLASS, []() -> TAGE::ECS::GameObject* { return new CLASS(); }); \
        } \
    } CLASS##FactoryRegisterInstance;