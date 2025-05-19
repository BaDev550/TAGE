#pragma once
#include "Asset.h"
#include "TAGE/Core/Utilities/Memory.h"

#include <unordered_map>
#include <string>
#include <type_traits>
#include "yaml-cpp/yaml.h"

#include "TAGE/Core/AssetManager/Importers/ModelImporter.h"

namespace TAGE::ASSET {
	class AssetManager
	{
	public:
		static MEM::Ref<Asset> ImportAsset(const std::filesystem::path& path);
		static MEM::Ref<Asset> GetAsset(const UUID& handle);
		static MEM::Ref<Asset> GetAsset(const std::string& path);
		static bool HasAsset(const UUID& handle);
		static bool HasAsset(const std::string& path);
		static void RegisterImporter(MEM::Ref<AssetImporter> importer);

	private:
		static std::unordered_map<UUID, MEM::Ref<Asset>> s_Assets;
		static std::unordered_map<std::string, MEM::Ref<AssetImporter>> s_Importers;
	};
}