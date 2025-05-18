#pragma once
#include "TAGE/TAGE.h"
#include <string>
#include <unordered_map>

namespace TAGE::EDITOR {
	class Panel {
	public:
		virtual void OnImGuiRender() = 0;
		virtual void End() {};

		uint32_t GetIcon(const std::string& name) {
			if (_Icons[name])
				return _Icons[name]->GetID();
		}

		MEM::Ref<TETexture2D> GetIconTexture(const std::string& name) {
			if (_Icons[name])
				return _Icons[name];
			return nullptr;
		}

		void AddIcon(const std::string& name, const std::string& path) {
			if (!_Icons[name]) {
				_Icons[name] = MEM::CreateRef<TETexture2D>(path);
			}
		}

		void AddIcon(const std::string& name, const MEM::Ref<TETexture2D>& texture) {
			if (!_Icons[name]) {
				_Icons[name] = texture;
			}
		}

		bool HasIcon(const std::string& name) {
			return _Icons.find(name) != _Icons.end();
		}

		const int IconSize = 16;
	private:
		std::unordered_map<std::string, MEM::Ref<TETexture2D>> _Icons;
	};
}