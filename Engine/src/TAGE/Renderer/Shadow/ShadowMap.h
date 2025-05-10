#pragma once

namespace TAGE::RENDERER {
	class ShadowMap
	{
	public:
		ShadowMap(uint32_t width, uint32_t height);

		void BindDepthTexture(int slot);
		void Bind() const;
		void Unbind() const;
		uint32_t GetDepthMap() const { return _DepthMap; }
	private:
		uint32_t _Framebuffer;
		uint32_t _DepthMap;
		uint32_t _Width, _Height;
	};
}