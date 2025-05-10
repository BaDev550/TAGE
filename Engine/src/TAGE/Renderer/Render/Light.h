#pragma once
#include <glm/glm.hpp>

namespace TAGE::RENDERER {
	struct Light {
		glm::vec3 Pos = { 1.0f, -1.0f, -0.5f };
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.0f;
	};

	struct DirectionalLight : public Light {
		glm::vec3 Direction = { -0.2f, -1.0f, -0.3f };
	};
}
