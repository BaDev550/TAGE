#include "tagepch.h"
#include "ShaderLibrary.h"

namespace TAGE::RENDERER {
    std::unordered_map<std::string, MEM::Ref<Shader>> ShaderLibrary::_Shaders;

    void ShaderLibrary::Add(const std::string& name, const MEM::Ref<Shader>& shader)
    {
        if (Exists(name))
        {
            CORE_LOG_WARN("Shader '{0}' already exists in ShaderLibrary. Overwriting.", name);
        }
        _Shaders[name] = shader;
    }

    MEM::Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    {
        _Shaders[name] = MEM::CreateRef<Shader>(vertexPath, fragmentPath);
        return _Shaders[name];
    }

    MEM::Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        if (!Exists(name))
        {
            CORE_LOG_ERROR("Shader '{0}' not found in ShaderLibrary.", name);
            return nullptr;
        }
        return _Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name)
    {
        if(_Shaders[name])
            return true;
        return false;
    }
}