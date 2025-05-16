#pragma once
#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "TAGE/Renderer/Model/AssimpGLMHelpers.h"
#include "Socket.h"

namespace TAGE::RENDERER {
	struct KeyPosition
	{
		glm::vec3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		glm::quat orientation;
		float timeStamp;
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;
	};

	class Bone
	{
	public:
		Bone(const std::string& name, int ID, const aiNodeAnim* channel = nullptr);

		void SetParentBone(Bone* parent) { m_ParentBone = parent; }
		void Update(float animationTime);
		glm::mat4 GetLocalTransform() { return m_LocalTransform; }
		std::string GetBoneName() const { return m_Name.c_str(); }
		int GetBoneID() { return m_ID; }

		int GetPositionIndex(float animationTime)
		{
			if (m_NumPositions == 0)
				return 0;

			for (int index = 0; index < m_NumPositions - 1; ++index)
			{
				if (animationTime < m_Positions[index + 1].timeStamp)
					return index;
			}
			return m_NumPositions - 2;
		}

		int GetRotationIndex(float animationTime)
		{
			if (m_NumRotations == 0)
				return 0;

			for (int index = 0; index < m_NumRotations - 1; ++index)
			{
				if (animationTime < m_Rotations[index + 1].timeStamp)
					return index;
			}
			return m_NumRotations - 2;
		}

		int GetScaleIndex(float animationTime)
		{
			if (m_NumScalings == 0)
				return 0;

			for (int index = 0; index < m_NumScalings - 1; ++index)
			{
				if (animationTime < m_Scales[index + 1].timeStamp)
					return index;
			}
			return m_NumScalings - 2;
		}

		void AddSocket(Socket& socket) { 
			socket.SetParent(this);
			m_Sockets.push_back(socket);
		}

        Socket* GetSocket(const std::string& name) const {  
           for (auto& socket : m_Sockets)
               if (socket.GetSocketName() == name)  
                   return const_cast<Socket*>(&socket);
           return nullptr;  
        }

		glm::mat4 GetFinalTransform() const {
			return m_FinalTransform;
		}

		std::vector<Socket> GetAllSockets() const { return m_Sockets; }

		void SetChannel(const aiNodeAnim* channel) {
			m_Positions.clear();
			m_Rotations.clear();
			m_Scales.clear();

			m_NumPositions = channel->mNumPositionKeys;
			for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
			{
				aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
				float timeStamp = channel->mPositionKeys[positionIndex].mTime;
				KeyPosition data;
				data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
				data.timeStamp = timeStamp;
				m_Positions.push_back(data);
			}

			m_NumRotations = channel->mNumRotationKeys;
			for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
			{
				aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
				float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
				KeyRotation data;
				data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
				data.timeStamp = timeStamp;
				m_Rotations.push_back(data);
			}

			m_NumScalings = channel->mNumScalingKeys;
			for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
			{
				aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
				float timeStamp = channel->mScalingKeys[keyIndex].mTime;
				KeyScale data;
				data.scale = AssimpGLMHelpers::GetGLMVec(scale);
				data.timeStamp = timeStamp;
				m_Scales.push_back(data);
			}
		}
	private:

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
		{
			float scaleFactor = 0.0f;
			float midWayLength = animationTime - lastTimeStamp;
			float framesDiff = nextTimeStamp - lastTimeStamp;
			scaleFactor = midWayLength / framesDiff;
			return scaleFactor;
		}

		glm::mat4 InterpolatePosition(float animationTime)
		{
			if (m_NumPositions == 0)
				return glm::mat4(1.0f);
			if (m_NumPositions == 1)
				return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

			int p0Index = GetPositionIndex(animationTime);
			int p1Index = p0Index + 1;
			if (p1Index >= m_Positions.size())
				p1Index = p0Index;

			float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
				m_Positions[p1Index].timeStamp, animationTime);

			glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position,
				m_Positions[p1Index].position, scaleFactor);

			return glm::translate(glm::mat4(1.0f), finalPosition);
		}


		glm::mat4 InterpolateRotation(float animationTime)
		{
			if (m_NumRotations == 0)
				return glm::mat4(1.0f);
			if (m_NumRotations == 1)
				return glm::toMat4(glm::normalize(m_Rotations[0].orientation));

			int p0Index = GetRotationIndex(animationTime);
			int p1Index = p0Index + 1;
			if (p1Index >= m_Rotations.size())
				p1Index = p0Index;

			float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
				m_Rotations[p1Index].timeStamp, animationTime);

			glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation,
				m_Rotations[p1Index].orientation, scaleFactor);

			return glm::toMat4(glm::normalize(finalRotation));
		}

		glm::mat4 InterpolateScaling(float animationTime)
		{
			if (m_NumScalings == 0)
				return glm::mat4(1.0f);
			if (m_NumScalings == 1)
				return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

			int p0Index = GetScaleIndex(animationTime);
			int p1Index = p0Index + 1;
			if (p1Index >= m_Scales.size())
				p1Index = p0Index;

			float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
				m_Scales[p1Index].timeStamp, animationTime);

			glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale,
				m_Scales[p1Index].scale, scaleFactor);

			return glm::scale(glm::mat4(1.0f), finalScale);
		}


		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScalings;
		std::vector<Socket> m_Sockets;

		glm::mat4 m_FinalTransform;
		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;
		Bone* m_ParentBone = nullptr;
	};
}