#pragma once

#include "TAGE/ECS/Scene/Scene.h"
#include "fstream"

#ifdef SCENE_SAVE_FORMAT_YAML
#endif

#ifdef SCENE_SAVE_FORMAT_JSON
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
#endif

namespace TAGE::ECS::IO {
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene* scene)
			: _Scene(scene) {
		}

		void Save(const std::string& path);
		void Load(const std::string& path);
	private:
#ifdef SCENE_SAVE_FORMAT_JSON
		void SaveToJson(const std::string& path);
		void LoadFromJson(const std::string& path);
		json SerializeGameObject(ECS::Actor* actor);
		json SerializeBaseActor(ECS::Actor* actor);
		void DeserializeActor(const json& data);
#elif defined(SCENE_SAVE_FORMAT_YAML)
		void SaveToYaml(const std::string& path);
#endif

	private:
		Scene* _Scene;

	};
}