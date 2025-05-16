#pragma once
#include <glm/glm.hpp>
#include <vector>

struct BoneInfo
{
	int id;
	glm::mat4 offset;
};

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};