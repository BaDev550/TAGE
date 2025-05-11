#pragma once

#include "glm/glm.hpp"

namespace TAGE::RAYHELPER {
    glm::vec3 ScreenPointToRay(
        const glm::vec2& screenPos,
        float screenWidth,
        float screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection);
}