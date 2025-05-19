#pragma once
#include <string>
#include "TAGE/Core/Utilities/UUID.h"

namespace TAGE::ASSET {
	enum class AssetType : uint8_t {
		NONE = 0,
		TEXTURE,
		MODEL,
		MATERIAL,
		SHADER,
		AUDIO,
		ANIMATION,
		SKELETON,
		SCENE,
		PREFAB
	};

	struct AssetFileHeader {
		char magic[8] = "TAGEAST";
		uint32_t version = 1;
		AssetType type;
		UUID handle;
		uint32_t size;
	};

	class Asset {
	public:
		virtual ~Asset() = default;
		virtual AssetType GetType() const = 0;
		UUID handle = 0;
		std::string path;
	};
}