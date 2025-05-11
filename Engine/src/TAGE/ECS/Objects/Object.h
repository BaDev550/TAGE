#pragma once

#include <string>

namespace TAGE::ECS {
    class Object {
    public:
        Object() = default;
        virtual ~Object() = default;

        virtual std::string GetName() const { return "Object"; }
        virtual void Tick(float dt) {};
    };
}