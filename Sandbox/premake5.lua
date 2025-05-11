project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "../%{IncludeDir.Engine}",
        "../%{IncludeDir.GLFW}",
        "../%{IncludeDir.GLAD}",
        "../%{IncludeDir.ImGui}",
        "../%{IncludeDir.Assimp}",
        "../%{IncludeDir.entt}",
        "../%{IncludeDir.glm}",
        "../%{IncludeDir.log}",
        "../%{IncludeDir.json}",
        "../%{IncludeDir.Bullet}",
    }

    links {
        "Engine"
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
