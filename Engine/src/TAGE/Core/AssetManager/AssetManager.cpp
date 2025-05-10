#include "tagepch.h"
#include "AssetManager.h"

namespace TAGE::ASSET {
	std::unordered_map<std::string, std::shared_ptr<Asset>> AssetManager::_Assets;
}