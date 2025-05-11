#pragma once
#include "TAGE/Renderer/Shader/Shader.h"
#include "TAGE/Renderer/Buffers/VertexArrayBuffer.h"

namespace TAGE::RENDERER::DEBUG {
	class DebugRenderer
	{
	public:
		static void Init();
		static void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
	private:
		static MEM::Ref<Shader> _LineShader;
		static MEM::Ref<VertexArrayBuffer> DEBUGVAO;
	};
}