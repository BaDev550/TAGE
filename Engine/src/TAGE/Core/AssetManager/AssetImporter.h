#pragma once
#include <filesystem>
#include "TAGE/Core/AssetManager/Asset.h"
#include "yaml-cpp/yaml.h"

namespace TAGE::ASSET {
	class AssetImporter {
	public:
		virtual ~AssetImporter() = default;
		virtual MEM::Ref<Asset> Import(const std::filesystem::path& source, const std::filesystem::path& destination) = 0;
		virtual std::vector<std::string> GetSupportedExtensions() const = 0;
	};
}