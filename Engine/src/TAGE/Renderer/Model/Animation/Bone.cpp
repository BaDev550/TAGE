#include "tagepch.h"
#include "Bone.h"

namespace TAGE::RENDERER{
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		: m_Name(name), m_ID(ID), m_NumPositions(0), m_NumRotations(0), m_NumScalings(0), m_ParentBone(nullptr)
    {
        if (channel)
            SetChannel(channel);
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