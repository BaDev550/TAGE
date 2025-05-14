#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"
#include "TAGE/TEvents.h"

#include "imgui.h"
#include "GameObjects/Player/Player.h"
#include "GameObjects/Common/Wall.h"
#include "GameObjects/Common/Door.h"

class SandboxLayer : public TELayer {
public:
	SandboxLayer(TEApplication* instance) : TELayer("Sandbox Layer"), _AppInstance(instance) {
		_AppInstance->GetWindow().EnableCursor(_Cursor);
		_World = &_AppInstance->GetScene().GetWorld();

		camera = _World->SpawnActor("Camera");
		camera->AddComponent<TCameraComponent>(TCamType::EDITOR);

		Floor = new Wall("Assets/Models/Cube/Cube.obj", glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(50.0f, 0.5f, 50.0f));
		MainDoor = new Door("Assets/Models/Cube/Cube.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 2.0f, 1.0f));
	}
	~SandboxLayer() {
		delete Floor;
		delete MainDoor;
	}

	void OnImGuiRender() override {
		ImGui::Begin("Debug");

		float FPS = 1 / _AppInstance->GetDeltaTime();
		ImGui::Text("FPS: %f", FPS);

		if (ImGui::CollapsingHeader("Light")) {
			ImGui::DragFloat3("Light Dir", glm::value_ptr(_AppInstance->GetRenderer()._SceneLight.Pos));
			ImGui::ColorEdit3("Light Color", glm::value_ptr(_AppInstance->GetRenderer()._SceneLight.Color));
		}

		if (ImGui::CollapsingHeader("Scene")) {
			if (!_World)
				CORE_LOG_ERROR("No World");
			auto actors = _World->GetAllActors();

			if (ImGui::Button("Create Actor")) {
				std::string name = "Actor (" + std::to_string(actors.size()) + ")";
				TActor* tmp_actor = _World->SpawnActor(name);
			}

			for (auto actor : actors) {
				if (!actor || !actor->IsValid()) continue;

				if (ImGui::Selectable(actor->GetComponent<TTagComponent>().name.c_str())) {
					_SelectedActor = actor;
					std::cout << typeid(*_SelectedActor).name() << std::endl;
				}
			}

			ImGui::Text("Selected Actor: %s", _SelectedActor ? _SelectedActor->GetComponent<TTagComponent>().name.c_str() : "None");

			if (_SelectedActor) {
				if (_SelectedActor->GetParent()) {
					auto parentActor = _SelectedActor->GetParent();
					ImGui::Text("Parent is: %s", parentActor->GetComponent<TTagComponent>().name.c_str());
				}
				else {
					ImGui::Text("Parent is null!");
				}

				if (_SelectedActor->HasComponent<TTransformComponent>()) {
					auto& tc = _SelectedActor->GetComponent<TTransformComponent>();
					ImGui::Text("Transform");
					ImGui::DragFloat3("Position", glm::value_ptr(tc.Position), 0.1f);
					if (_SelectedActor->GetParent())
						ImGui::DragFloat3("Local Position", glm::value_ptr(tc.LocalPosition), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f);
				}
				if (_SelectedActor->HasComponent<TAGE::ECS::ColliderComponent>()) {
					auto& collider = _SelectedActor->GetComponent<TAGE::ECS::ColliderComponent>();

					ImGui::Text("Collider");

					const char* colliderTypes[] = { "Box", "Sphere", "Capsule"};
					const char* colliderRespondes[] = { "None", "Block", "Overlap" };
					int selectedCollider = static_cast<int>(collider.Shape);
					int selectedResponde = static_cast<int>(collider.ResponseType);
					if (ImGui::Combo("Collider Type", &selectedCollider, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
						collider.Shape = static_cast<TAGE::ECS::ColliderShapeType>(selectedCollider);
						collider.Dirty = true;
					}

					if (ImGui::Combo("Collider Responde", &selectedResponde, colliderRespondes, IM_ARRAYSIZE(colliderRespondes))) {
						collider.ResponseType = static_cast<TAGE::ECS::CollisionResponseType>(selectedResponde);
						collider.Dirty = true;
					}

					if (collider.Shape == TAGE::ECS::ColliderShapeType::BOX) {
						collider.Dirty |= ImGui::DragFloat3("Box Size", glm::value_ptr(collider.Size), 0.1f);
					}
					else if (collider.Shape == TAGE::ECS::ColliderShapeType::SPHERE) {
						collider.Dirty |= ImGui::DragFloat("Sphere Radius", &collider.Size.x, 0.1f);
					}
					else if (collider.Shape == TAGE::ECS::ColliderShapeType::CAPSULE) {
						collider.Dirty |= ImGui::DragFloat("Capsule Radius", &collider.Size.x, 0.1f);
						collider.Dirty |= ImGui::DragFloat("Capsule Height", &collider.Size.y, 0.1f);
					}

					if (_SelectedActor->HasComponent<TRigidBodyComponent>()) {
						auto& rb = _SelectedActor->GetComponent<TRigidBodyComponent>();
						const char* types[] = { "Static", "Dynamic", "Kinematic" };
						int currentType = static_cast<int>(rb.BodyType);

						if (ImGui::Combo("Body Type", &currentType, types, IM_ARRAYSIZE(types))) {
							rb.BodyType = static_cast<TAGE::ECS::PhysicsBodyType>(currentType);

							int flags = rb.Body->getCollisionFlags();
							flags &= ~(btCollisionObject::CF_KINEMATIC_OBJECT);

							switch (rb.BodyType) {
							case TAGE::ECS::PhysicsBodyType::STATIC:
								rb.Body->setMassProps(0.0f, btVector3(0, 0, 0));
								rb.Body->setActivationState(ISLAND_SLEEPING);
								break;
							case TAGE::ECS::PhysicsBodyType::DYNAMIC:
								rb.Body->setMassProps(1.0f, btVector3(1, 1, 1));
								rb.Body->setActivationState(ACTIVE_TAG);
								break;
							case TAGE::ECS::PhysicsBodyType::KINEMATIC:
								flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
								rb.Body->setActivationState(DISABLE_DEACTIVATION);
								break;
							}

							rb.Body->setCollisionFlags(flags);
						}

					}
				}
				if (ImGui::Button("Add Static Mesh")) {
					_SelectedActor->AddComponent<TStaticMeshComponent>("Assets/Models/Cube/cube.obj");
				}
				if (ImGui::Button("Add RigidBody")) {
					_AppInstance->GetPhysicsSystem().CreateRigidBody(_SelectedActor, 1.0f);
				}
				if (ImGui::Button("Add Collider")) {
					_SelectedActor->AddComponent<TAGE::ECS::ColliderComponent>();
				}
			}
		}
		ImGui::End();
	}

	bool OnKeyPressed(TEKeyPressedE& event) {
		if (event.GetKey() == TEKey::Escape) {
			GetApp().Close();
			return true;
		}
		if (event.GetKey() == TEKey::F1) {
			_Cursor = !_Cursor;
			_AppInstance->GetWindow().EnableCursor(_Cursor);
		}
		if (event.GetKey() == TEKey::F2) {
			_OnEditor = !_OnEditor;
			_AppInstance->SetEngineMode(_OnEditor ? TEEngineMode::EDITOR : TEEngineMode::GAME);
		}
		return false;
	}

	void OnEvent(TEEvent& event) override {
		TEEventDistpacher dispatcher(event);
		dispatcher.Dispatch<TEKeyPressedE>(BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
	}

private:
	TEApplication* _AppInstance;
	TEWorld* _World;

	TActor* _SelectedActor = nullptr;
	TActor* camera;
	Player player;

	Wall* Floor;
	Door* MainDoor;
	bool _Cursor = false;
	bool _OnEditor = false;
};

class Sandbox : public TEApplication {
public:
	Sandbox() : Application("Sandbox") {
		PushLayer(new SandboxLayer(this));
		PushLayer(new TEProfilerLayer());
    }

    ~Sandbox() override = default;
};

TEApplication* CreateApplication() {
    return new Sandbox();
}

int main() {
    auto app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}