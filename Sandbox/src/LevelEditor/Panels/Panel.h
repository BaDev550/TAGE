#pragma once
#include "TAGE/TAGE.h"
#include <string>
#include <unordered_map>

class Panel {
public:
	virtual ~Panel() {
		for (auto [name, texture] : _Icons)
			delete texture;
	};

	virtual void Begin() = 0;
	virtual void End() {};

	uint32_t GetIcon(const std::string& name) {
		if (_Icons[name])
			return _Icons[name]->GetID();
	}

	void AddIcon(const std::string& name, const std::string& path) {
		if (!_Icons[name]) {
			_Icons[name] = new TETexture2D(path);
		}
	}

	const int IconSize = 16;
private:
	std::unordered_map<std::string, TETexture2D*> _Icons;
};