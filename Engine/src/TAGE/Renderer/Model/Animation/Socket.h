#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace TAGE::RENDERER {

    struct KeyPosition;
    struct KeyRotation;
    struct KeyScale;
    class Bone;

    class Socket
    {
    public:
        Socket(const std::string& name, int ID);

        void SetParent(Bone* parent);
        Bone* GetParent() const { return m_Parent; }
        void AddPositionKeyframe(const glm::vec3& position, float timeStamp);
        void AddRotationKeyframe(const glm::vec3& rotation, float timeStamp);
        void AddScaleKeyframe(const glm::vec3& scale, float timeStamp);

        void Update(float animationTime, const glm::mat4& parentTransform);

        glm::vec3 GetPosition() const;
        glm::vec3 GetWorldPosition() const;
        glm::vec3 GetRotation() const;
        glm::vec3 GetScale() const;
        glm::mat4 GetLocalTransform() const;

        std::string GetSocketName() const;
        int GetSocketID() const;

    private:
        int GetPositionIndex(float animationTime) const;
        int GetRotationIndex(float animationTime) const;
        int GetScaleIndex(float animationTime) const;

        glm::mat4 InterpolatePosition(float animationTime) const;
        glm::mat4 InterpolateRotation(float animationTime) const;
        glm::mat4 InterpolateScaling(float animationTime) const;

        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale> m_Scales;

        int m_NumPositions = 0;
        int m_NumRotations = 0;
        int m_NumScalings = 0;

        glm::mat4 m_LocalTransform = glm::mat4(1.0f);
        glm::mat4 m_WorldTransform{ 1.0f };
        std::string m_Name;
        int m_ID;

        Bone* m_Parent = nullptr;
    };
}
