#pragma once
#include "TAGE/Core/Defines.h"
#include <vector>

namespace TAGE::RENDERER
{
	enum class ShaderDataType : uint8_t {
		None = 0,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
		Int,
		Bool
	};
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::None: return 0;
			case ShaderDataType::Float: return 4;
			case ShaderDataType::Vec2:  return 4 * 2;
			case ShaderDataType::Vec3:  return 4 * 3;
			case ShaderDataType::Vec4:  return 4 * 4;
			case ShaderDataType::Mat3:  return 4 * 3 * 3;
			case ShaderDataType::Mat4:  return 4 * 4 * 4;
			case ShaderDataType::Int:   return 4;
			case ShaderDataType::Bool:  return 4;
		default:
			ENGINE_ASSERT(false, "Wrong ShaderDataType");
			return 0;
		}
		return 0;
	}
	
	#define MAX_BONE_INFLUENCE 4

	struct Vertex{
		float Position[3];
		float TexCoords[2];
		float Normal[3];
		float Tangent[3];
		float Bitangent[3];
		int BoneIDs[MAX_BONE_INFLUENCE];
		float Weights[MAX_BONE_INFLUENCE];
	};
	
	struct BufferElement{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		uint32_t Divisor = 0;
		bool Normalized;
	
		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, uint32_t divisor = 0, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized), Divisor(divisor)
		{ 	}
	
		uint32_t GetComponentCount() const {
			switch (Type)
			{
				case ShaderDataType::None:  return 0;
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Vec2: return 2;
				case ShaderDataType::Vec3: return 3;
				case ShaderDataType::Vec4: return 4;
				case ShaderDataType::Mat3: return 3 * 3;
				case ShaderDataType::Mat4: return 4 * 4;
				case ShaderDataType::Int:  return 1;
				case ShaderDataType::Bool: return 1;
			default:
				ENGINE_ASSERT(false, "Wrong ShaderDataType");
				return 0;
			}
			return 0;
		}
	};
	class BufferLayout {
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: _Elements(elements) {
			CalculateOffsetAndStride();
		}
	
		inline const std::vector<BufferElement>& GetElements() const { return _Elements; }
		inline uint32_t GetStride() const { return _Stride; }
	
		std::vector<BufferElement>::iterator begin() { return _Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return _Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return _Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return _Elements.end(); }
	private:
		void CalculateOffsetAndStride() {
			uint32_t offset = 0;
			_Stride = 0;
			for (auto& element : _Elements) {
				element.Offset = offset;
				offset += element.Size;
				_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> _Elements;
		uint32_t _Stride = 0;
	};
	
	class VertexBuffer {
	public:
		VertexBuffer(void* vertices, uint32_t size);
		~VertexBuffer();
	
		void Bind() const;
		void Unbind() const;
	
		void SetLayout(const BufferLayout& layout) { _Layout = layout; }
		void SetData(void* vertices, uint32_t size);
		const BufferLayout& GetLayout() const { return _Layout; }
	private:
		uint32_t _BufferID;
		BufferLayout _Layout;
	};
	
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();
	
		void Bind() const;
		void Unbind() const;
		uint32_t GetCount() const { return _Count; };
	private:
		uint32_t _BufferID;
		uint32_t _Count;
	};
}