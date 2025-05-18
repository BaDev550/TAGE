#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace TAGE::PROJECT {
    struct ProjectData {
        std::string ProjectName;
        std::string Version;
        std::string ContentDir;
        std::string SavedDir;
        std::string ConfigDir;
        std::string SettingsFile;
    };

	class ProjectCreator
	{
	public:
		ProjectCreator(const std::string& name);
		ProjectCreator() {}
		~ProjectCreator() {}

		static ProjectData LoadProjectData(const std::string& projectPath);
        static void SaveProject(const ProjectData& data, const std::string& path);

		static void CreateProject(const std::string& name);
		static void CreateProjectFiles();
		static void SetRootPath(const std::filesystem::path& path) { rootPath = path; }
		static const std::filesystem::path& GetRootPath() { return rootPath; }
		static const std::filesystem::path& GetContentPath() { return rootPath / data.ContentDir; }

	private:
		inline static std::filesystem::path rootPath;
		inline static ProjectData data;
	};
}