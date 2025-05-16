#pragma once

#include "TAGE/Core/Utilities/Memory.h"

#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Animator.h"
#include "TAGE/Renderer/Camera.h"
#include "TAGE/Core/AssetManager/AssetManager.h"

namespace TAGE::ECS {
	using namespace RENDERER;
	enum class ECameraType { EDITOR, PERSFECTIVE };

	struct MeshComponent
	{
	public:
		MeshComponent(const std::string& modelPath, EMeshType type = EMeshType::STATIC) {
			meshType = type;
			model = MEM::CreateScope<Model>(modelPath, meshType);
		}
		MeshComponent(EMeshType type = EMeshType::STATIC) { meshType = type; model = MEM::CreateScope<Model>(type); }

		void Draw(MEM::Ref<Shader> shader, const glm::mat4& transform) {
			if (IsVisible) {
				shader->Bind();

				model->Draw(transform, shader);
			}
		}

		void LoadModel(const std::string& modelPath) {
			model->Reset();
			model = MEM::CreateScope<Model>(modelPath, meshType);
		}

		Model* GetModel() const { return model ? model.get() : nullptr; }
		bool IsVisible = true;
	private:
		MEM::Scope<Model> model;
		EMeshType meshType = EMeshType::STATIC;
	};

	struct StaticMeshComponent : public MeshComponent
	{
		StaticMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::STATIC) {
		}
		StaticMeshComponent() : MeshComponent(EMeshType::STATIC) {}
	};

	struct SkeletalMeshComponent : public MeshComponent
	{
		SkeletalMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::SKELETAL) {
		}
		SkeletalMeshComponent() : MeshComponent(EMeshType::SKELETAL) {}

		MEM::Ref<Skeletal> GetSkeleton() const { return GetModel()->GetSkeletal(); }
	};

	struct AnimatorComponent
	{
		Animator* GetInstance() const { return AnimatorInstance ? AnimatorInstance.get() : nullptr; }

		AnimatorComponent() = default;
		AnimatorComponent(const MEM::Ref<Skeletal>& skeleton, const MEM::Ref<Animation>& animation) { AnimatorInstance = MEM::CreateRef<Animator>(skeleton, animation); }
	private:
		MEM::Ref<Animator> AnimatorInstance;
	};

	struct CameraComponent {
	public:
		CameraComponent(ECameraType type) {
			switch (type)
			{
			case ECameraType::EDITOR: _Camera = MEM::CreateRef<EditorCamera>(45.0f, 1.0f, 0.1f, 100.0f); break;
			case ECameraType::PERSFECTIVE: _Camera = MEM::CreateRef<PerspectiveCamera>(45.0f, 1.0f, 0.1f, 100.0f); break;
			}
			_Type = type;
		}

		glm::mat4 GetViewMatrix() const { return GetCamera()->GetViewMatrix(); }
		glm::mat4 GetProjectionMatrix() const { return GetCamera()->GetProjectionMatrix(); }
		glm::vec3 GetForward() { return GetCamera()->GetFront(); }

		MEM::Ref<Camera> GetCamera() const { if (_Camera) return _Camera; }
		ECameraType GetType() const { return _Type; }
		void SetType(ECameraType type) { _Type = type; }
	private:
		MEM::Ref<Camera> _Camera;
		ECameraType _Type;
	};
}