#include "tagepch.h"
#include "Bone.h"

namespace TAGE::RENDERER{
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
        : m_Name(name), m_ID(ID), m_LocalTransform(1.0f)
    {
        if (channel)
        {
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
        else
        {
            m_NumPositions = m_NumRotations = m_NumScalings = 0;
        }
    }
	void Bone::Update(float animationTime) {
		glm::mat4 translation = InterpolatePosition(animationTime);
		glm::mat4 rotation = InterpolateRotation(animationTime);
		glm::mat4 scale = InterpolateScaling(animationTime);

		m_LocalTransform = translation * rotation * scale;

		if (m_ParentBone)
			m_FinalTransform = m_ParentBone->m_FinalTransform * m_LocalTransform;
		else
			m_FinalTransform = m_LocalTransform;

		for (auto& socket : m_Sockets)
			socket.Update(animationTime, m_FinalTransform);
	}
}