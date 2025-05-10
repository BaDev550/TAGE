#pragma once
#include "Asset.h"
#include "TAGE/Core/Utilities/Memory.h"

#include <unordered_map>
#include <string>
#include <type_traits>

namespace TAGE::ASSET {
	class AssetManager
	{
	public:
		template<typename T>
		static MEM::Ref<T> Load(const std::string& path);

		template<typename T>
		static MEM::Ref<T> Get(const std::string& path);

		template<typename T>
		static bool Has(const std::string& path);

		template<typename T>
		static void Reload(const std::string& path);
	private:
		static std::unordered_map<std::string, MEM::Ref<Asset>> _Assets;
	};

	template<typename T>
	MEM::Ref<T> AssetManager::Load(const std::string& path) {
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

		auto it = _Assets.find(path);
		if (it != _Assets.end())
			return std::dynamic_pointer_cast<T>(_Assets[path]);

		MEM::Ref<T> asset = MEM::CreateRef<T>(path);
		_Assets[path] = std::dynamic_pointer_cast<Asset>(asset);
		return asset;
	}

	template<typename T>
	MEM::Ref<T> AssetManager::Get(const std::string& path) {
		auto it = _Assets.find(path);
		if (it == _Assets.end())
			return nullptr;

		return std::dynamic_pointer_cast<T>(_Assets[path]);
	}

	template<typename T>
	bool AssetManager::Has(const std::string& path) {
		return (_Assets.find(path) != _Assets.end());
	}

	template<typename T>
	void AssetManager::Reload(const std::string& path) {
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");
		_Assets[path] = std::dynamic_pointer_cast<Asset>(MEM::CreateRef<T>(path));
	}
}