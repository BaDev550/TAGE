#include "tagepch.h"
#include "DebugRenderer.h"
#include "TAGE/Renderer/Buffers/Buffers.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "GLAD/glad.h"

namespace TAGE::RENDERER::DEBUG {
	MEM::Ref<Shader> DebugRenderer::_LineShader;
	MEM::Ref<VertexArrayBuffer> DebugRenderer::DEBUGVAO;

	struct DebugVertex {
		glm::vec3 Pos;
		glm::vec2 Color;
	};

	void DebugRenderer::Init()
	{
		_LineShader = ShaderLibrary::Get("line");

		std::vector<DebugVertex> vertices = { };
		std::vector<uint32_t> indices = { 0, 1 };

		MEM::Ref<VertexBuffer> vertexBuffer = MEM::CreateRef<VertexBuffer>(vertices.data(), vertices.size() * sizeof(DebugVertex));
		MEM::Ref<IndexBuffer> indexBuffer = MEM::CreateRef<IndexBuffer>(indices.data(), indices.size());

		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aColor" }
		};
		vertexBuffer->SetLayout(layout);

		DEBUGVAO = MEM::CreateRef<VertexArrayBuffer>();
		DEBUGVAO->AddVertexBuffer(vertexBuffer);
		DEBUGVAO->SetIndexBuffer(indexBuffer);
	}

	void DebugRenderer::DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
	{
		std::vector<DebugVertex> vertices = {
			{ from, color },
			{ to,   color }
		};
		MEM::Ref<VertexBuffer> vertexBuffer = MEM::CreateRef<VertexBuffer>(vertices.data(), vertices.size() * sizeof(DebugVertex));
		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aColor" }
		};
		vertexBuffer->SetLayout(layout);

		DEBUGVAO->AddVertexBuffer(vertexBuffer);

		_LineShader->Bind();
		DEBUGVAO->Bind();
		glLineWidth(1.0f);
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
	}
}