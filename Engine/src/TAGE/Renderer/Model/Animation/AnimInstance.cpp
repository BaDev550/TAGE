#include "tagepch.h"
#include "AnimInstance.h"

namespace TAGE::RENDERER {
	AnimInstance::AnimInstance(ECS::GameObject* owner, const MEM::Ref<Model>& model) {
		_Owner = owner;
		_Model = model;
		_Skeletal = model->GetSkeletal();
		_Animator = _Owner->AddComponent<ECS::AnimatorComponent>(_Skeletal, nullptr);
	}

	void AnimInstance::Play(const std::string& name) {
		_Animator.GetInstance()->PlayAnimation(_Animations[name]);
	}

	void AnimInstance::PlayBlend(const std::string& name, float duration) {
		_Animator.GetInstance()->PlayAnimationWithBlend(_Animations[name], duration);
	}

	void AnimInstance::LoadAnimation(const std::string& name, const std::string& path) {
		_Animations[name] = MEM::CreateRef<Animation>(path, _Model);
	}

	void AnimInstance::SetParameter(const std::string& name, float value) {
		_FloatParams[name] = value;
	}
	void AnimInstance::SetParameter(const std::string& name, bool  value) {
		_BoolParams[name] = value;
	}
	void AnimInstance::SetParameter(const std::string& name, glm::vec3 value) {
		_Vec3Params[name] = value;
	}

	float AnimInstance::GetFloatParam(const std::string& name) const {
		auto it = _FloatParams.find(name);
		return it != _FloatParams.end() ? it->second : 0.0f;
	}

	bool AnimInstance::GetBoolParam(const std::string& name) const {
		auto it = _BoolParams.find(name);
		return it != _BoolParams.end() ? it->second : false;
	}

	glm::vec3 AnimInstance::GetVec3Param(const std::string& name) const {
		auto it = _Vec3Params.find(name);
		return it != _Vec3Params.end() ? it->second : glm::vec3(0.0f);
	}

	void AnimInstance::UpdateAnimInstance(float dt) {
		_Animator.GetInstance()->UpdateAnimation(dt);

		Update(dt);

		_Skeletal->Update(_Animator.GetInstance()->GetCurrentAnimationTime());
	}
}