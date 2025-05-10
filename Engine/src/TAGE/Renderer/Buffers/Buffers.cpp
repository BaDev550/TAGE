#include "tagepch.h"
#include "Buffers.h"
#include <GLAD/glad.h>

namespace TAGE::RENDERER
{
	VertexBuffer::VertexBuffer(void* vertices, uint32_t size)
	{
		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteVertexArrays(1, &_BufferID);
	}
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
	}
	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::SetData(void* vertices, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : _Count(count)
	{
		glCreateBuffers(1, &_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteVertexArrays(1, &_BufferID);
	}
	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _BufferID);
	}
	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}