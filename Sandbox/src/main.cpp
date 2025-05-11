#include "TAGE/TAGE.h"
#include "imgui.h"

class IInteractable {
public:
	virtual void Interact() = 0;
};

enum class ObjectType {
	OBJECT_DOOR,
	OBJECT_CHARACTER
};
class GameObject : public TActor {
public:
	GameObject(std::string name, ObjectType type, TEWorld& world) : TActor(world.SpawnActor(name)) {}
	virtual ~GameObject() = default;
	ObjectType GetType() const { return _Type; }

private:
	ObjectType _Type;
};

enum class EDoorState {
	DOOR_OPENED,
	DOOR_CLOSED
};
class Door : public GameObject, public IInteractable {
public:
	Door(std::string name, TEWorld& world) : GameObject(name, ObjectType::OBJECT_DOOR, world) {
		AddComponent<TStaticMeshComponent>("Assets/Models/Cube/cube.obj");
		auto collider = AddComponent<TAGE::ECS::ColliderComponent>();

		world.GetPhysicsSystem().CreateRigidBody(this, 1.0f);
		auto rb = GetComponent<TRigidBodyComponent>();

		rb.BodyType = TAGE::ECS::PhysicsBodyType::Kinematic;
		int flags = rb.Body->getCollisionFlags();
		flags &= ~(btCollisionObject::CF_KINEMATIC_OBJECT);
		flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
		rb.Body->setActivationState(DISABLE_DEACTIVATION);
	}

	virtual void Interact() override {
		if (_state == EDoorState::DOOR_OPENED)
			Close();
		else
			Open();
	}

	void Open() { _state = EDoorState::DOOR_OPENED; LOG_INFO("Door Opened"); }
	void Close() { _state = EDoorState::DOOR_CLOSED; LOG_INFO("Door Closed"); }

	void OnCreate() override {
		LOG_INFO("Door created");
	}
	void Tick(float dt) override {

	}

	EDoorState _state;
};

class OakDoor : public Door {
public:
	OakDoor(TEWorld& world) : Door("oakDoor", world) {}

	void Tick(float dt) override {

	}

	EDoorState _state;
};

class SandboxLayer : public TELayer {
public:
	SandboxLayer(TEApplication* instance) : TELayer("Sandbox Layer"), _AppInstance(instance) {
		_World = &_AppInstance->GetScene().GetWorld();

		camera = _World->SpawnActor("Camera");
		camera->AddComponent<TCameraComponent>(TCamType::Editor);
		mainDoor = new Door("Door", * _World);
		oakDoor = new OakDoor(*_World);

		TActor* actor = _World->SpawnActor("Arms");
		actor->AddComponent<TSkeletalMeshComponent>("Assets/Models/Arms/Arms.glb");

		//TEAnim anim1("Assets/Models/Arms/Walk.glb", actor->GetComponent<TSkeletalMeshComponent>().model.get());
		//actor->AddComponent<TAnimatorComponent>(&anim1);
	}
	~SandboxLayer() {
		delete mainDoor;
		delete oakDoor;
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

				if (ImGui::Selectable(actor->GetComponent<TTagComponent>().tag.c_str())) {
					_SelectedActor = actor;
				}
			}

			ImGui::Text("Selected Actor: %s", _SelectedActor ? _SelectedActor->GetComponent<TTagComponent>().tag.c_str() : "None");

			if (_SelectedActor) {
				if (_SelectedActor->HasComponent<TTransformComponent>()) {
					auto& tc = _SelectedActor->GetComponent<TTransformComponent>();
					ImGui::Text("Transform");
					ImGui::DragFloat3("Position", glm::value_ptr(tc.Position), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f);
				}
				if (_SelectedActor->HasComponent<TAGE::ECS::ColliderComponent>()) {
					auto& collider = _SelectedActor->GetComponent<TAGE::ECS::ColliderComponent>();

					ImGui::Text("Collider");

					const char* colliderTypes[] = { "Box", "Sphere", "Capsule"};
					int selectedCollider = static_cast<int>(collider.Shape);
					if (ImGui::Combo("Collider Type", &selectedCollider, colliderTypes, IM_ARRAYSIZE(colliderTypes))) {
						collider.Shape = static_cast<TAGE::ECS::ColliderShapeType>(selectedCollider);
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
							case TAGE::ECS::PhysicsBodyType::Static:
								rb.Body->setMassProps(0.0f, btVector3(0, 0, 0));
								rb.Body->setActivationState(ISLAND_SLEEPING);
								break;
							case TAGE::ECS::PhysicsBodyType::Dynamic:
								rb.Body->setMassProps(1.0f, btVector3(1, 1, 1));
								rb.Body->setActivationState(ACTIVE_TAG);
								break;
							case TAGE::ECS::PhysicsBodyType::Kinematic:
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
				
				if (dynamic_cast<Door*>(_SelectedActor)) {
					auto door = dynamic_cast<Door*>(_SelectedActor);
					std::string doorstat;
					switch (door->_state)
					{
					case EDoorState::DOOR_OPENED: doorstat = "Opened"; break;
					case EDoorState::DOOR_CLOSED: doorstat = "Closed"; break;
						break;
					}
					ImGui::Text("Door Stat: %s", doorstat.c_str());
				}
			}
		}

		//if (ImGui::CollapsingHeader("Post Process")) {
		//	ImGui::DragFloat("Exposure", &_AppInstance->GetRenderer()._PostProcessEffects.Exposure, 0.01);
		//}
		ImGui::End();
	}

	bool OnKeyPressed(TEKeyPressedE& event) {
		if (event.GetKey() == TEKey::Escape) {
			TEApplication::Get().Close();
			return true;
		}
		return false;
	}

	bool OnMouseButtonPressed(TEMouseButtonPressedE& event) {
		if (event.GetButton() == TEMouseButton::Left) {
			auto& activeCam = camera->GetComponent<TCameraComponent>();

			glm::vec2 mousePos = TEInput::GetMousePosition();

			glm::vec3 rayDirection = TAGE::RAYHELPER::ScreenPointToRay(mousePos, 1280, 720, activeCam.GetViewMatrix(), activeCam.GetProjectionMatrix());;
			glm::vec3 rayOrigin = activeCam.Camera->GetPosition();
			glm::vec3 rayEnd = rayOrigin + rayDirection * 1000.0f;

			TEHitResult hit = TECast::Raycast(rayOrigin, rayEnd, TERayDrawType::FOR_DURATION, 5.0f);

			if (hit.hit && hit.actor && hit.actor->HasComponent<TTagComponent>()) {
				_SelectedActor = hit.actor;
				
				auto interactable = dynamic_cast<IInteractable*>(hit.actor);
				if (interactable) {
					interactable->Interact();
				}
			}
			return true;
		}
		return false;
	}

	void OnEvent(TEEvent& event) override {
		TEEventDistpacher dispatcher(event);
		dispatcher.Dispatch<TEKeyPressedE>(BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
		dispatcher.Dispatch<TEMouseButtonPressedE>(BIND_EVENT_FN(SandboxLayer::OnMouseButtonPressed));
	}

private:
	TEApplication* _AppInstance;
	TEWorld* _World;

	TActor* _SelectedActor = nullptr;
	TActor* camera;
	Door* mainDoor;
	OakDoor* oakDoor;
};

class Sandbox : public TEApplication {
public:
	Sandbox() : Application("Sandbox") {
		PushLayer(new SandboxLayer(this));
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