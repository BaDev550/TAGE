#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Shader/Shader.h"
#include <unordered_map>

namespace TAGE::RENDERER {
    class ShaderLibrary {
    public:
        static void Add(const std::string& name, const MEM::Ref<Shader>& shader);
        static MEM::Ref<Shader> Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
        static MEM::Ref<Shader> Get(const std::string& name);
        static bool Exists(const std::string& name);
    private:
        static std::unordered_map<std::string, MEM::Ref<Shader>> _Shaders;
    };
}