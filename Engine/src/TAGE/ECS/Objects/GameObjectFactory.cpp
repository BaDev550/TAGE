#include "tagepch.h"
#include "GameObjectFactory.h"
#include "GameObject.h"

namespace TAGE::ECS {
    std::unordered_map<std::string, GameObjectFactory::CreatorFunc>& GameObjectFactory::GetRegistry() {
        static std::unordered_map<std::string, CreatorFunc> registry;
        return registry;
    }

    void GameObjectFactory::Register(const std::string& className, CreatorFunc func) {
        GetRegistry()[className] = func;
    }

    GameObject* GameObjectFactory::Create(const std::string& className) {
        auto& registry = GetRegistry();
        auto it = registry.find(className);
        if (it != registry.end()) {
            return it->second();
        }
        return nullptr;
    }
}