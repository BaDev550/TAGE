#pragma once

#include "TAGE/TMeshes.h"
#include "TAGE/TObjects.h"

class PlayerAnimInstance : public TEAnimInstance {
public:
	PlayerAnimInstance(TGameObject* owner, const TAGE::MEM::Ref<TEMesh>& model) : TEAnimInstance(owner, model) {

	}

	virtual void Update(float dt) {
		glm::vec3 aimDir = glm::normalize(GetVec3Param("CameraForward"));
		aimDir.y = 0.0f;

		TEBone* bone = _Skeletal->GetBone("mixamorig1:Spine2");
		if (bone) {
			glm::mat4 modelMatrix = _Owner->GetTransformMatrix();
			glm::mat4 boneWorld = _Skeletal->GetBoneWorldTransform("mixamorig1:Spine2", modelMatrix);
			glm::vec3 boneForward = glm::normalize(glm::vec3(boneWorld[2]));

			glm::quat aimRot = glm::rotation(boneForward, aimDir);

			glm::mat4& localTransform = bone->GetLocalTransform();
			localTransform = glm::mat4_cast(aimRot) * localTransform;
		}

		if (GetFloatParam("PlayerSpeed") > 10.0f)
			PlayBlend("Idle", 0.3);
	}
};