#pragma once

#include "World.h"

#define SCENE_SAVE_FORMAT_JSON
namespace TAGE::ECS {
	class Scene
	{
	public:
        Scene(const std::string& name = "Untitled");
        Scene() : _Name("Untitled") {}

        void OnUpdate(float deltaTime, SystemUpdateMode mode);

        World& GetWorld() { return _World; }
        const std::string& GetName() const { return _Name; }
    private:
        std::string _Name;
        std::string _Path;
        World _World;
	};
}