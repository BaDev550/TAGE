#pragma once
#include <glm/glm.hpp>

namespace TAGE::RENDERER {
	struct Light {
		glm::vec3 Pos = { -3.5, 11.0f, -3.5 };
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float Intensity = 1.7f;
	};

	struct DirectionalLight : public Light {
		glm::vec3 Direction = { -0.2f, -1.0f, -0.3f };
	};
}
