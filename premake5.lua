workspace "TAGE"
   architecture "x64"
   startproject "Sandbox"

   configurations
   {
      "Debug",
      "Release"
   }

   outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

   IncludeDir = {}
   IncludeDir["Engine"] = "Engine/src"
   IncludeDir["GLFW"] = "Engine/thirdParty/GLFW/include"
   IncludeDir["GLAD"] = "Engine/thirdParty/GLAD/include"
   IncludeDir["ImGui"] = "Engine/thirdParty/imgui"
   IncludeDir["ImGizmo"] = "Engine/thirdParty/ImGuizmo"
   IncludeDir["Assimp"] = "Engine/thirdParty/assimp/include"
   IncludeDir["entt"] =  "Engine/thirdParty/entt/include"
   IncludeDir["glm"] =  "Engine/thirdParty/glm/"
   IncludeDir["log"] = "Engine/thirdParty/spdlog/"
   IncludeDir["json"] =  "Engine/thirdParty/json/"
   IncludeDir["Bullet"] = "Engine/thirdParty/bullet-physics/src"

   group "Dependencies"
      include "Engine/thirdParty/imgui"
      include "Engine/thirdParty/ImGuizmo/"
      include "Engine/thirdParty/GLAD"
      include "Engine/thirdParty/GLFW"
      include "Engine/thirdParty/assimp/"
      include "Engine/thirdParty/bullet-physics/"
   group ""

   group "Engine"
      include "Engine"
   group ""
    
   group "Game"
      include "Sandbox"
   group ""