#include "TAGE/TAGE.h"
#include "imgui.h"

class SandboxLayer : public TELayer {
public:
	SandboxLayer(TEApplication* instance) : TELayer("Sandbox Layer"), _AppInstance(instance) {
		_World = &_AppInstance->GetScene().GetWorld();

		TActor* camera = _World->SpawnActor("Camera");
		camera->AddComponent<TCameraComponent>(TCamType::Editor);

		TActor* actor = _World->SpawnActor("Test");
		actor->AddComponent<TSkeletalMeshComponent>("Remy/Remy.fbx");

		//TEAnim anim1("Remy/Walking.fbx", actor->GetComponent<TSkeletalMeshComponent>().model.get());
		//actor->AddComponent<TAnimatorComponent>(&anim1);
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

			if (_SelectedActor) {
				if (_SelectedActor->HasComponent<TTransformComponent>()) {
					auto& tc = _SelectedActor->GetComponent<TTransformComponent>();
					ImGui::Text("Transform");
					ImGui::DragFloat3("Position", glm::value_ptr(tc.Position), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f);
				}
				if (ImGui::Button("Add Static Mesh")) {
					_SelectedActor->AddComponent<TStaticMeshComponent>("Cube/cube.obj");
				}
			}
		}

		//if (ImGui::CollapsingHeader("Post Process")) {
		//	ImGui::DragFloat("Exposure", &_AppInstance->GetRenderer()._PostProcessEffects.Exposure, 0.01);
		//}
		ImGui::End();
	}

	bool OnKeyPressed(TEKeyReleasedE& event) {
		if (event.GetKey() == TEKey::Escape) {
			TEApplication::Get().Close();
			return true;
		}
		return false;
	}

	void OnEvent(TEEvent& event) override {
		TEEventDistpacher dispatcher(event);
		dispatcher.Dispatch<TEKeyReleasedE>(BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
	}

private:
	TEApplication* _AppInstance;
	TEWorld* _World;

	TActor* _SelectedActor = nullptr;
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