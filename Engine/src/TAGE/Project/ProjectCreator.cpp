#include "tagepch.h"
#include "ProjectCreator.h"

namespace TAGE::PROJECT {
#ifdef YAML_CPP_DLL
#pragma message("YAML_CPP_DLL is defined")
#endif

	namespace fs = std::filesystem;

	ProjectCreator::ProjectCreator(const std::string& name)
	{
		CreateProject(name);
	}

	ProjectData ProjectCreator::LoadProjectData(const std::string& projectPath)
	{
		YAML::Node config = YAML::LoadFile(projectPath);

		ProjectData data;
		data.ProjectName = config["ProjectName"].as<std::string>();
		data.Version = config["Version"].as<std::string>();
		data.ContentDir = config["ContentDir"].as<std::string>();
		data.SavedDir = config["SavedDir"].as<std::string>();
		data.ConfigDir = config["ConfigDir"].as<std::string>();
		data.SettingsFile = config["SettingsFile"].as<std::string>();

		return data;
	}

	void ProjectCreator::SaveProject(const ProjectData& data, const std::string& path) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "ProjectName" << YAML::Value << data.ProjectName;
		out << YAML::Key << "Version" << YAML::Value << data.Version;
		out << YAML::Key << "ContentDir" << YAML::Value << data.ContentDir;
		out << YAML::Key << "SavedDir" << YAML::Value << data.SavedDir;
		out << YAML::Key << "ConfigDir" << YAML::Value << data.ConfigDir;
		out << YAML::Key << "SettingsFile" << YAML::Value << data.SettingsFile;
		out << YAML::EndMap;

		std::ofstream fout(path + ".tageproj");
		fout << out.c_str();
	}

	void ProjectCreator::CreateProject(const std::string& name)
	{
		rootPath = fs::current_path() / name;
		data.ProjectName = name;
		data.Version = "1.0.0";
		data.ContentDir = "Content";
		data.SavedDir = "Saved";
		data.ConfigDir = "Config";
		data.SettingsFile = "settings.ini";
		CreateProjectFiles();
	}

	void ProjectCreator::CreateProjectFiles()
	{
		fs::create_directories(rootPath);
		fs::create_directories(rootPath / data.ContentDir / "Assets");
		fs::create_directories(rootPath / data.ContentDir / "Scenes");
		fs::create_directories(rootPath / data.ContentDir / "Shaders");
		fs::create_directories(rootPath / data.SavedDir   / "Cache/Shaders");
		fs::create_directories(rootPath / data.ConfigDir);

		std::ofstream iniFile(rootPath / data.ConfigDir / data.SettingsFile);
		iniFile << "[General]\n";
		iniFile << "WindowWidth=1280\n";
		iniFile << "WindowHeight=720\n";
		iniFile << "VSync=true\n";
		iniFile << "Fullscreen=false\n";
		iniFile.close();

		SaveProject(data, (rootPath / data.ProjectName).string());
	}
}