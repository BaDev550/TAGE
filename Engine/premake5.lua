project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "tagepch.h"
    pchsource "src/tagepch.cpp"

    files {
        "src/**.h",
        "src/**.cpp",
        "thirdParty/stb/**.h",
        "thirdParty/stb/**.cpp",
        "thirdParty/glm/glm/**.hpp",
        "thirdParty/glm/glm/**.inl",
    }

    includedirs {
        "src",
        "thirdParty",
        "thirdParty/spdlog/",
        "../%{IncludeDir.Engine}",
        "../%{IncludeDir.GLFW}",
        "../%{IncludeDir.GLAD}",
        "../%{IncludeDir.ImGui}",
        "../%{IncludeDir.ImGizmo}",
        "../%{IncludeDir.Assimp}",
        "../%{IncludeDir.entt}",
        "../%{IncludeDir.glm}",
        "../%{IncludeDir.log}",
        "../%{IncludeDir.json}",
        "../%{IncludeDir.Bullet}",
        "../%{IncludeDir.yamlcpp}"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    links {
            "opengl32.lib",
            "ImGui",
            "ImGuiGizmo",
            "GLAD",
            "GLFW",
            "Assimp",
            "BulletDynamics",
            "BulletCollision",
            "LinearMath",
            "yaml-cpp"
        }

    buildoptions "/utf-8"
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
