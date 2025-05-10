#include "tagepch.h"
#include "VertexArrayBuffer.h"
#include "glad/glad.h"

namespace TAGE::RENDERER
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Vec2: return GL_FLOAT;
		case ShaderDataType::Vec3: return GL_FLOAT;
		case ShaderDataType::Vec4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		default:
			ENGINE_ASSERT(false, "Wrong ShaderDataType");
			return 0;
		}
		return 0;
	}

	VertexArrayBuffer::VertexArrayBuffer()
	{
		glGenVertexArrays(1, &_BufferID);
	}
	VertexArrayBuffer::~VertexArrayBuffer()
	{
		glDeleteVertexArrays(1, &_BufferID);
	}
	void VertexArrayBuffer::Bind() const
	{
		glBindVertexArray(_BufferID);
	}
	void VertexArrayBuffer::Unbind() const
	{
		glBindVertexArray(0);
	}
	void VertexArrayBuffer::AddVertexBuffer(const MEM::Ref<VertexBuffer>& vertexBuffer)
	{
		uint32_t _AttribIndex = 0;
		ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(_BufferID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			GLuint componentCount = element.GetComponentCount();
			GLenum type = ShaderDataTypeToOpenGLBaseType(element.Type);
			GLboolean normalized = element.Normalized ? GL_TRUE : GL_FALSE;
			GLsizei stride = layout.GetStride();
			const void* offset = (const void*)(intptr_t)element.Offset;
#ifdef DEBUG_BUFFER
			std::cout << "Binding attribute at index "
				<< " with component count " << element.GetComponentCount()
				<< " and type " << ShaderDataTypeToOpenGLBaseType(element.Type) << std::endl;
#endif
			if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4)
			{
				uint8_t rows = (element.Type == ShaderDataType::Mat3) ? 3 : 4;
				uint8_t cols = rows;

				for (uint8_t i = 0; i < rows; ++i)
				{
					glEnableVertexAttribArray(_AttribIndex);
					glVertexAttribPointer(
						_AttribIndex,
						rows,
						type,
						normalized,
						stride,
						(const void*)(intptr_t)(element.Offset + sizeof(float) * rows * i)
					);
					if (element.Divisor > 0)
						glVertexAttribDivisor(_AttribIndex, element.Divisor);
					_AttribIndex++;
				}
			}
			else
			{
				glEnableVertexAttribArray(_AttribIndex);
				glVertexAttribPointer(
					_AttribIndex,
					componentCount,
					type,
					normalized,
					stride,
					offset
				);
				if (element.Divisor > 0)
					glVertexAttribDivisor(_AttribIndex, element.Divisor);
				_AttribIndex++;
			}
		}

		_VertexBuffers.push_back(vertexBuffer);
	}
	void VertexArrayBuffer::SetIndexBuffer(const MEM::Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(_BufferID);
		indexBuffer->Bind();
		_IndexBuffer = indexBuffer;
	}
}