#include "tagepch.h"
#include "Scene.h"

namespace TAGE::ECS {
    Scene::Scene(const std::string& name)
        : _Name(name)
    {
        _World.Init();
    }

    void Scene::OnUpdate(float deltaTime, SystemUpdateMode mode)
    {
        _World.Update(deltaTime, mode);
    }
}