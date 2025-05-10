#pragma once

#include "TAGE/Renderer/Buffers/Buffers.h"
#include "TAGE/Core/Utilities/Memory.h"

namespace TAGE::RENDERER
{
	class VertexArrayBuffer
	{
	public:
		VertexArrayBuffer();
		~VertexArrayBuffer();
	
		void Bind() const;
		void Unbind() const;
	
		void AddVertexBuffer(const MEM::Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const MEM::Ref<IndexBuffer>& indexBuffer);
		void SetVertexCount(uint32_t count) { _VertexCount = count; };
		MEM::Ref<IndexBuffer> GetIndexBuffer() const { return _IndexBuffer; };
		uint32_t GetVertexCount() const { return _VertexCount; };
	private:
		std::vector<MEM::Ref<VertexBuffer>> _VertexBuffers;
		MEM::Ref<IndexBuffer> _IndexBuffer;
		uint32_t _VertexCount;
		uint32_t _BufferID;
		uint32_t _AttribIndex = 0;
	};
}