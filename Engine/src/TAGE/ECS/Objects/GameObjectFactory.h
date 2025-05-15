#pragma once
#include <string>
#include <unordered_map>
#include <functional>

namespace TAGE::ECS {
    class GameObject;

    class GameObjectFactory {
    public:
        using CreatorFunc = std::function<GameObject* ()>;

        static void Register(const std::string& className, CreatorFunc func);
        static GameObject* Create(const std::string& className);

    private:
        static std::unordered_map<std::string, CreatorFunc>& GetRegistry();
    };
}