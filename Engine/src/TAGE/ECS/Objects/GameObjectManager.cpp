#include "tagepch.h"
#include "GameObjectManager.h"
#include "GameObject.h"

namespace TAGE::ECS {
    void GameObjectManager::RegisterGameObject(GameObject* object){
        if (object) {
            _gameObjects.push_back(object);
        }
    }
    void GameObjectManager::RemoveGameObject(GameObject* object){
        if (object) {
            _gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
        }
    }
    void GameObjectManager::UpdateGameObjects(float deltaTime){
        for (auto& gameObject : _gameObjects) {
            gameObject->Tick(deltaTime);
        }
    }

    void GameObjectManager::ClearGameObjects()
    {
        _gameObjects.clear();
    }

    GameObject* GameObjectManager::GetGameObjectByType(ObjectType type){
        for (auto& gameObject : _gameObjects) {
            if (gameObject->GetType() == type) {
                return gameObject;
            }
        }
        return nullptr;
    }
}