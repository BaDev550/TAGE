#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <functional>
#include "AnimData.h"
#include "Bone.h"
#include "TAGE/Renderer/Model/Model.h"

namespace TAGE::RENDERER {
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& animationPath, Model* model);
		~Animation() {
		}

		Bone* FindBone(const std::string& name)
		{
			auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
				[&](const Bone& Bone)
				{
					return Bone.GetBoneName() == name;
				}
			);
			if (iter == m_Bones.end()) return nullptr;
			else return &(*iter);
		}

		inline float GetTicksPerSecond() { return m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }

	private:
		void ReadMissingBones(const aiAnimation* animation, Model& model)
		{
			int size = animation->mNumChannels;

			auto& boneInfoMap = model.GetBoneInfoMap();
			int& boneCount = model.GetBoneCount();

			for (int i = 0; i < size; i++)
			{
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.C_Str();

				if (boneInfoMap.find(boneName) == boneInfoMap.end())
				{
					boneInfoMap[boneName].id = boneCount;
					boneCount++;
				}
				m_Bones.push_back(Bone(channel->mNodeName.C_Str(),
					boneInfoMap[channel->mNodeName.C_Str()].id, channel));
			}

			m_BoneInfoMap = boneInfoMap;
		}

		void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
		{
			ENGINE_ASSERT(src, "Node is empty");

			dest.name = src->mName.data;
			dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
			dest.childrenCount = src->mNumChildren;

		}
		float m_Duration = 0.0f;
		int m_TicksPerSecond;
		std::vector<Bone> m_Bones;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}