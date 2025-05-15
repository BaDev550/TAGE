#include "tagepch.h"
#include "SceneSerializer.h"
#include "TAGE/ECS/ECS/Systems/System.h"
#include "TAGE/Core/Application/Application.h"
#include "TAGE/ECS/Objects/GameObjectFactory.h"
#include <regex>

namespace TAGE::ECS::IO {
	void SceneSerializer::Save(const std::string& path)
	{
#ifdef SCENE_SAVE_FORMAT_JSON
		SaveToJson(path);
#endif
	}

	void SceneSerializer::Load(const std::string& path)
	{
#ifdef SCENE_SAVE_FORMAT_JSON
		LoadFromJson(path);
#endif
	}

#ifdef SCENE_SAVE_FORMAT_JSON
	void SceneSerializer::SaveToJson(const std::string& path)
	{
		ordered_json j;

		j["Scene"]["Name"] = _Scene->GetName();
		auto& actorsArray = j["Scene"]["GameObjects"];

		for (Actor* actor : _Scene->GetWorld().GetAllActors()) {
			if (actor->IsGameObject()) {
				auto* obj = static_cast<GameObject*>(actor);
				actorsArray.push_back(SerializeGameObject(obj));
			}
			else {
				actorsArray.push_back(SerializeGameObject(actor));
			}
		}

		std::ofstream file(path);
		std::string pretty = j.dump(2);

		std::regex vectorPattern(R"(\[\s+([^\[\]\n]+?),\s+([^\[\]\n]+?),\s+([^\[\]\n]+?)\s+\])");
		pretty = std::regex_replace(pretty, vectorPattern, "[$1, $2, $3]");

		file << pretty;
		file.close();
	}
	json SceneSerializer::SerializeGameObject(ECS::Actor* actor) {
		using namespace ECS;
		ordered_json j = SerializeBaseActor(actor);

		if (dynamic_cast<ECS::GameObject*>(actor)){
			ECS::GameObject* obj = dynamic_cast<ECS::GameObject*>(actor);
			j["Type"] = static_cast<int>(obj->GetType());
			j["Class"] = obj->GetClassNameW();
		}

		return j;
	}

	json SceneSerializer::SerializeBaseActor(ECS::Actor* actor)
	{
		ordered_json j;
		j["Entity"] = static_cast<int>(actor->GetEntity());
		j["Name"] = actor->GetComponent<TagComponent>().name;
		j["Tags"] = actor->GetComponent<TagComponent>().tags;

		auto& comps = j["Components"];

		if (actor->HasComponent<TransformComponent>()) {
			const auto& t = actor->GetComponent<TransformComponent>();
			comps["TransformComponent"] = {
				{"Position", { t.Position.x, t.Position.y, t.Position.z }},
				{"Rotation", { t.Rotation.x, t.Rotation.y, t.Rotation.z }},
				{"Scale",    { t.Scale.x,    t.Scale.y,    t.Scale.z }}
			};
		}

		if (actor->HasComponent<StaticMeshComponent>()) {
			const auto& mesh = actor->GetComponent<StaticMeshComponent>();
			comps["StaticMeshComponent"] = {
				{"Path", mesh.GetModel() ? mesh.GetModel()->GetPath() : ""}
			};
		}

		if (actor->HasComponent<SkeletalMeshComponent>()) {
			const auto& mesh = actor->GetComponent<SkeletalMeshComponent>();
			comps["SkeletalMeshComponent"] = {
				{"Path", mesh.GetModel() ? mesh.GetModel()->GetPath() : ""}
			};
		}

		if (actor->HasComponent<AnimatorComponent>()) {
			const auto& anim = actor->GetComponent<AnimatorComponent>();
			auto* animInst = anim.GetInstance();
			if (animInst && animInst->GetCurrentAnimation()) {
				comps["AnimatorComponent"] = {
					{"Animation", animInst->GetCurrentAnimation()->GetPath()}
				};
			}
		}

		if (actor->HasComponent<ColliderComponent>()) {
			const auto& col = actor->GetComponent<ColliderComponent>();
			comps["ColliderComponent"] = {
				{"Shape", static_cast<int>(col.Shape)},
				{"Size", { col.Size.x, col.Size.y, col.Size.z }},
				{"ResponseType", static_cast<int>(col.ResponseType)}
			};
		}

		if (actor->HasComponent<RigidBodyComponent>()) {
			const auto& rb = actor->GetComponent<RigidBodyComponent>();
			comps["RigidBodyComponent"] = {
				{"Type", static_cast<int>(rb.BodyType)},
				{"Mass", static_cast<float>(rb.Body->getMass())}
			};
		}

		if (actor->HasComponent<CameraComponent>()) {
			const auto& cc = actor->GetComponent<CameraComponent>();
			comps["CameraComponent"] = {
				{"Type", static_cast<int>(cc.GetType())}
			};
		}

		return j;
	}

	void SceneSerializer::LoadFromJson(const std::string& path)
	{
		Application::Get().SetEngineMode(SystemUpdateMode::LOADING);

		_Scene->GetWorld().Clear();
		_Scene->GetWorld().GetGameObjectManager().ClearGameObjects();

		CORE_LOG_INFO("Loading Scene: {}", path);
		std::ifstream file(path);
		if (!file.is_open()) {
			CORE_LOG_ERROR("Failed to open scene file: {}", path);
			return;
		}

		json j;
		file >> j;
		file.close();

		const auto& sceneData = j["Scene"];
		const auto& actorsArray = sceneData["GameObjects"];

		for (const auto& actorData : actorsArray) {
			DeserializeActor(actorData);
		}
		CORE_LOG_INFO("Scene: {} Loaded", path);
		Application::Get().SetEngineMode(SystemUpdateMode::GAME);
	}
	void SceneSerializer::DeserializeActor(const json& data)
	{
		std::string name = data.value("Name", "UnnamedActor");
		ObjectType type = static_cast<ObjectType>(data["Type"]);

		GameObject* gameObject = GameObjectFactory::Create(data["Class"]);
		if (gameObject) {
			_Scene->GetWorld().GetGameObjectManager().RegisterGameObject(gameObject);

			if (data.contains("Tags")) {
				if (!gameObject->HasComponent<TagComponent>()) {
					gameObject->AddComponent<TagComponent>();
				}
				auto& tagsJson = data["Tags"];
				auto& tags = gameObject->GetComponent<TagComponent>().tags;
				tags.clear();
				for (auto& tag : tagsJson) {
					tags.push_back(tag.get<std::string>());
				}
			}


			const auto& comps = data["Components"];

			if (comps.contains("TransformComponent")) {
				const auto& t = comps["TransformComponent"];
				auto& tc = gameObject->GetComponent<TransformComponent>();
				tc.Position = glm::vec3(t["Position"][0], t["Position"][1], t["Position"][2]);
				tc.Rotation = glm::vec3(t["Rotation"][0], t["Rotation"][1], t["Rotation"][2]);
				tc.Scale = glm::vec3(t["Scale"][0], t["Scale"][1], t["Scale"][2]);
			}

			if (comps.contains("StaticMeshComponent")) {
				if (gameObject->HasComponent<StaticMeshComponent>()) {
					auto& mesh = gameObject->GetComponent<StaticMeshComponent>();
					std::string path = comps["StaticMeshComponent"]["Path"];
					if (!path.empty()) {
						mesh.LoadModel(path);
					}
				}
				else {
					std::string path = comps["StaticMeshComponent"]["Path"];
					if (!path.empty()) {
						gameObject->AddComponent<StaticMeshComponent>(path);
					}
				}
			}

			if (comps.contains("SkeletalMeshComponent")) {
				std::string path = comps["SkeletalMeshComponent"]["Path"];
				if (gameObject->HasComponent<SkeletalMeshComponent>()) {
					auto& mesh = gameObject->GetComponent<SkeletalMeshComponent>();
					if (!path.empty()) {
						mesh.LoadModel(path);
					}

					if (comps.contains("AnimatorComponent")) {
						std::string animPath = comps["AnimatorComponent"]["Animation"];
						if (!animPath.empty()) {
							auto skeletal = mesh.GetSkeleton();
							if (!gameObject->HasComponent<AnimatorComponent>()) {
								auto animation = MEM::CreateRef<Animation>(animPath, mesh.GetModel());
								gameObject->AddComponent<AnimatorComponent>(skeletal, animation);
							}
							else {
								auto& animator = gameObject->GetComponent<AnimatorComponent>();
							}
						}
					}
				}
				else {
					if (!path.empty()) {
						auto& mesh = gameObject->AddComponent<SkeletalMeshComponent>(path);
						if (comps.contains("AnimatorComponent")) {
							std::string animPath = comps["AnimatorComponent"]["Animation"];
							if (!animPath.empty()) {
								auto skeletal = mesh.GetSkeleton();
								auto animation = MEM::CreateRef<Animation>(animPath, mesh.GetModel());
								gameObject->AddComponent<AnimatorComponent>(skeletal, animation);
							}
						}
					}
				}
			}

			if (comps.contains("ColliderComponent")) {
				const auto& c = comps["ColliderComponent"];
				if (gameObject->HasComponent<ColliderComponent>()) {
					auto& cc = gameObject->GetComponent<ColliderComponent>();
					cc.Shape = static_cast<ColliderShapeType>(c["Shape"].get<int>());
					cc.Size = glm::vec3(c["Size"][0], c["Size"][1], c["Size"][2]);
					cc.ResponseType = static_cast<CollisionResponseType>(c["ResponseType"].get<int>());
				}
				else {
					auto& cc = gameObject->AddComponent<ColliderComponent>();
					cc.Shape = static_cast<ColliderShapeType>(c["Shape"].get<int>());
					cc.Size = glm::vec3(c["Size"][0], c["Size"][1], c["Size"][2]);
					cc.ResponseType = static_cast<CollisionResponseType>(c["ResponseType"].get<int>());
				}
			}

			if (comps.contains("RigidBodyComponent")) {
				if (!gameObject->HasComponent<RigidBodyComponent>()) {
					_Scene->GetWorld().GetPhysicsSystem().CreateRigidBody(gameObject, comps["RigidBodyComponent"]["Mass"].get<float>());
				}
				if (gameObject->HasComponent<RigidBodyComponent>()) {
					auto& rb = gameObject->GetComponent<RigidBodyComponent>();
					rb.BodyType = static_cast<PhysicsBodyType>(comps["RigidBodyComponent"]["Type"].get<int>());
				}
			}

			if (comps.contains("CameraComponent")) {
				if (!gameObject->HasComponent<CameraComponent>()) {
					auto& cc = gameObject->AddComponent<CameraComponent>(static_cast<ECameraType>(data["CameraComponent"]["Type"].get<int>()));
				}
			}
		}
	}
#elif defined(SCENE_SAVE_FORMAT_YAML)
	void SceneSerializer::SaveToYaml(const std::string& path)
	{
	}
#endif
}