#include "tagepch.h"
#include "RaycastUtils.h"

namespace TAGE::RAYHELPER {
    glm::vec3 ScreenPointToRay(const glm::vec2& screenPos, float screenWidth, float screenHeight, const glm::mat4& view, const glm::mat4& projection)
    {
        float x = (2.0f * screenPos.x) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * screenPos.y) / screenHeight;
        glm::vec4 rayClip(x, y, -1.0f, 1.0f);

        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

        glm::vec3 rayWorld = glm::vec3(glm::inverse(view) * rayEye);
        return glm::normalize(rayWorld);
    }
}