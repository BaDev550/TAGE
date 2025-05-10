#pragma once
#include <string>

namespace TAGE::ASSET {
	enum class AssetType {
		NONE = 0,
		SHADER,
		MATERIAL,
		TEXTURE,
		MODEL,
		AUDIO
	};

	class Asset {
	public:
		virtual ~Asset() = default;
		virtual AssetType GetType() const = 0;
	};
}