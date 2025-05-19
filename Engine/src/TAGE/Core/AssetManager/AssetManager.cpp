#include "tagepch.h"
#include "AssetManager.h"
#include <fstream>
#include "TAGE/Project/ProjectCreator.h"

namespace TAGE::ASSET {
    std::unordered_map<std::string, MEM::Ref<AssetImporter>> AssetManager::s_Importers;
	std::unordered_map<UUID, MEM::Ref<Asset>> AssetManager::s_Assets;

    void AssetManager::RegisterImporter(MEM::Ref<AssetImporter> importer)
    {
        for (const std::string& ext : importer->GetSupportedExtensions()) {
            s_Importers[ext] = importer;
        }
    }

    MEM::Ref<Asset> AssetManager::ImportAsset(const std::filesystem::path& source) {
        std::string ext = source.extension().string();

        if (s_Importers.find(ext) != s_Importers.end()) {
            auto& importer = s_Importers[ext];

            std::filesystem::path destination = PROJECT::ProjectCreator::GetRootPath() / "Content" / "Assets/Imported" / source.stem().string();
            CORE_LOG_INFO("Imported asset: {}", source.string());
			auto asset = importer->Import(source, destination);
			s_Assets[asset->handle] = asset;
            return asset;
        }
        else {
            CORE_LOG_WARN("No importer registered for extension: {}", ext);
            return nullptr;
        }
    }

    MEM::Ref<Asset> AssetManager::GetAsset(const UUID& handle)
    {
		auto it = s_Assets.find(handle);
		if (it != s_Assets.end()) {
			return it->second;
		}
		return nullptr;
    }

    MEM::Ref<Asset> AssetManager::GetAsset(const std::string& path)
    {
		for (const auto& pair : s_Assets)
			if (pair.second->path == path)
				return pair.second;

        return nullptr;
    }

    bool AssetManager::HasAsset(const UUID& handle)
    {
		return s_Assets.find(handle) != s_Assets.end();
    }

    bool AssetManager::HasAsset(const std::string& path)
    {
		for (const auto& pair : s_Assets)
			if (pair.second->path == path)
				return true;

        return false;
    }

}