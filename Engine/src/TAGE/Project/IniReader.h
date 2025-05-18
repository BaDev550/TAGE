#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

namespace TAGE::INI {
    class IniFile
    {
    public:
        bool Load(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open()) return false;

            std::string line;
            std::string currentSection;

            while (std::getline(file, line))
            {
                if (line.empty() || line[0] == ';' || line[0] == '#') continue;

                if (line[0] == '[')
                {
                    size_t end = line.find(']');
                    if (end != std::string::npos)
                        currentSection = line.substr(1, end - 1);
                }
                else
                {
                    size_t eqPos = line.find('=');
                    if (eqPos == std::string::npos) continue;

                    std::string key = line.substr(0, eqPos);
                    std::string value = line.substr(eqPos + 1);

                    std::string fullKey = currentSection.empty() ? key : currentSection + "." + key;
                    m_Data[fullKey] = value;
                }
            }

            return true;
        }

        std::string Get(const std::string& key, const std::string& defaultValue = "") const
        {
            auto it = m_Data.find(key);
            if (it != m_Data.end())
                return it->second;
            return defaultValue;
        }

        void Set(const std::string& key, const std::string& value)
        {
            m_Data[key] = value;
        }

        bool Save(const std::string& path)
        {
            std::ofstream file(path);
            if (!file.is_open()) return false;

            for (auto& [key, value] : m_Data)
            {
                file << key << "=" << value << "\n";
            }
            return true;
        }

    private:
        std::unordered_map<std::string, std::string> m_Data;
    };
}