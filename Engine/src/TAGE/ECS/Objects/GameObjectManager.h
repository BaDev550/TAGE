#pragma once
#include <vector>
#include <memory>
#include <algorithm>

namespace TAGE::ECS {
    class GameObject;
    enum class ObjectType;
    class GameObjectManager {
    public:
        void RegisterGameObject(GameObject* object);
        void RemoveGameObject(GameObject* object);
        void UpdateGameObjects(float deltaTime);
        void ClearGameObjects();
        std::vector<GameObject*> GetGameObjects() const { return _gameObjects; };

        GameObject* GetGameObjectByType(ObjectType type);

    private:
        std::vector<GameObject*> _gameObjects;
    };
}