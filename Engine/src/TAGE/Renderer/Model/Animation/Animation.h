#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <functional>
#include "AnimData.h"
#include "Bone.h"
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Skeletal.h"

namespace TAGE::RENDERER {
	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& animationPath, Model* model);
		~Animation() {
		}

		void SetLoop(bool loop) { m_Loop = loop; }

		inline float GetTicksPerSecond() { return m_TicksPerSecond * 0.5f; }
		inline float GetDuration() { return m_Duration; }
		inline std::string GetPath() const { return _Path; }

		inline bool IsLooping() { return m_Loop; }

		inline AssimpNodeData GetRootNode() const { m_Skeletal->m_RootNode; }
	private:
		void Animation::ReadMissingBones(const aiAnimation* animation) {
			for (int i = 0; i < animation->mNumChannels; i++) {
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.C_Str();

				Bone* bone = m_Skeletal->GetBone(boneName);
				if (bone) {
					bone->SetChannel(channel);
				}
			}
		}

		// Animation Propeties
		float m_Duration = 0.0f;
		bool m_Loop = false;
		int m_TicksPerSecond;
		std::string _Path;

		MEM::Ref<Skeletal> m_Skeletal;
	};
}