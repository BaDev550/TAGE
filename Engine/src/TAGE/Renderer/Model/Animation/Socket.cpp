#include "tagepch.h"
#include "Socket.h"
#include "Bone.h"

namespace TAGE::RENDERER {

    Socket::Socket(const std::string& name, int ID)
        : m_Name(name), m_ID(ID), m_Parent(nullptr), m_LocalTransform(1.0f) {
        if (m_Positions.empty()) {
            AddPositionKeyframe(glm::vec3(1.0f), 0.0f);
        }
        if (m_Rotations.empty()) {
            AddRotationKeyframe(glm::vec3(1.0f), 0.0f);
        }
        if (m_Scales.empty()) {
            AddScaleKeyframe(glm::vec3(1.0f), 0.0f);
        }
    }

    void Socket::SetParent(Bone* parent) {
        m_Parent = parent;
    }

    void Socket::AddPositionKeyframe(const glm::vec3& position, float timeStamp) {
        m_Positions.push_back({ position, timeStamp });
        m_NumPositions = static_cast<int>(m_Positions.size());
    }

    void Socket::AddRotationKeyframe(const glm::vec3& rotation, float timeStamp) {
        m_Rotations.push_back({ rotation, timeStamp });
        m_NumRotations = static_cast<int>(m_Rotations.size());
    }

    void Socket::AddScaleKeyframe(const glm::vec3& scale, float timeStamp) {
        m_Scales.push_back({ scale, timeStamp });
        m_NumScalings = static_cast<int>(m_Scales.size());
    }

    void Socket::Update(float animationTime, const glm::mat4& parentTransform) {
        m_LocalTransform = glm::mat4(1.0f);
        glm::mat4 translation, rotation, scale;

        if (m_NumPositions > 0) {
            translation = InterpolatePosition(animationTime);
        }

        if (m_NumRotations > 0) {
            rotation = InterpolateRotation(animationTime) * m_LocalTransform;
        }

        if (m_NumScalings > 0) {
            scale = InterpolateScaling(animationTime) * m_LocalTransform;
        }

        m_LocalTransform = (translation * rotation * scale) * parentTransform;
    }

    glm::vec3 Socket::GetPosition() const {
        return glm::vec3(m_LocalTransform[3]);
    }

    glm::vec3 Socket::GetWorldPosition() const {
        glm::mat4 world = m_LocalTransform;
        if (m_Parent)
            world = m_Parent->GetLocalTransform() * world;
        return glm::vec3(world[3]);
    }

    glm::vec3 Socket::GetRotation() const {
        glm::quat rotation = glm::quat_cast(m_LocalTransform);
        return glm::eulerAngles(rotation);
    }

    glm::vec3 Socket::GetScale() const {
        return glm::vec3(
            glm::length(glm::vec3(m_LocalTransform[0])),
            glm::length(glm::vec3(m_LocalTransform[1])),
            glm::length(glm::vec3(m_LocalTransform[2]))
        );
    }

    glm::mat4 Socket::GetLocalTransform() const {
        return m_LocalTransform;
    }

    std::string Socket::GetSocketName() const {
        return m_Name;
    }

    int Socket::GetSocketID() const {
        return m_ID;
    }

    int Socket::GetPositionIndex(float animationTime) const {
        for (int i = 0; i < m_NumPositions - 1; ++i)
            if (animationTime < m_Positions[i + 1].timeStamp)
                return i;
        return 0;
    }

    int Socket::GetRotationIndex(float animationTime) const {
        for (int i = 0; i < m_NumRotations - 1; ++i)
            if (animationTime < m_Rotations[i + 1].timeStamp)
                return i;
        return 0;
    }

    int Socket::GetScaleIndex(float animationTime) const {
        for (int i = 0; i < m_NumScalings - 1; ++i)
            if (animationTime < m_Scales[i + 1].timeStamp)
                return i;
        return 0;
    }

    glm::mat4 Socket::InterpolatePosition(float animationTime) const {
        if (m_NumPositions == 0)
            return glm::mat4(1.0f);

        if (m_NumPositions == 1)
            return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

        int index = GetPositionIndex(animationTime);
        if (index + 1 >= m_NumPositions)
            return glm::translate(glm::mat4(1.0f), m_Positions.back().position);

        float scale = (animationTime - m_Positions[index].timeStamp) /
            (m_Positions[index + 1].timeStamp - m_Positions[index].timeStamp);
        glm::vec3 final = glm::mix(m_Positions[index].position, m_Positions[index + 1].position, scale);
        return glm::translate(glm::mat4(1.0f), final);
    }

    glm::mat4 Socket::InterpolateRotation(float animationTime) const {
        if (m_NumRotations == 0)
            return glm::mat4(1.0f);

        if (m_NumRotations == 1)
            return glm::toMat4(glm::normalize(m_Rotations[0].orientation));

        int index = GetRotationIndex(animationTime);
        if (index + 1 >= m_NumRotations)
            return glm::toMat4(glm::normalize(m_Rotations.back().orientation));

        float scale = (animationTime - m_Rotations[index].timeStamp) /
            (m_Rotations[index + 1].timeStamp - m_Rotations[index].timeStamp);
        glm::quat final = glm::slerp(m_Rotations[index].orientation, m_Rotations[index + 1].orientation, scale);
        return glm::toMat4(glm::normalize(final));
    }

    glm::mat4 Socket::InterpolateScaling(float animationTime) const {
        if (m_NumScalings == 0)
            return glm::mat4(1.0f);

        if (m_NumScalings == 1)
            return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

        int index = GetScaleIndex(animationTime);
        if (index + 1 >= m_NumScalings)
            return glm::scale(glm::mat4(1.0f), m_Scales.back().scale);

        float scale = (animationTime - m_Scales[index].timeStamp) /
            (m_Scales[index + 1].timeStamp - m_Scales[index].timeStamp);
        glm::vec3 final = glm::mix(m_Scales[index].scale, m_Scales[index + 1].scale, scale);
        return glm::scale(glm::mat4(1.0f), final);
    }
}
