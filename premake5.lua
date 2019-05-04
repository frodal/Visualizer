workspace "Visualizer"
	architecture "x64"
	startproject "Visualizer"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Dependencies/GLFW-3.3/include"
IncludeDir["GLEW"] = "Dependencies/GLEW-2.1.0/include"
IncludeDir["ImGui"] = "Visualizer/src/vendor/imgui"

--include "Hazel/vendor/GLFW"
--include "Hazel/vendor/Glad"
--include "Hazel/vendor/imgui"

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
-- Visualizer                                                                           --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------

project "Visualizer"
	location "Visualizer"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	pchheader "PreCompiledHeader.h"
	pchsource "Visualizer/src/PreCompiledHeader.cpp"

	files
	{
		"%{prj.name}/src/*.h",
        "%{prj.name}/src/*.cpp",
        "%{prj.name}/src/tests/**.h",
        "%{prj.name}/src/tests/**.cpp",
        "%{prj.name}/src/vendor/*.h",
        "%{prj.name}/src/vendor/*.cpp",
        "%{prj.name}/src/vendor/imgui/*.h",
		"%{prj.name}/src/vendor/imgui/*.cpp",
        "%{prj.name}/src/vendor/glm/glm/**.hpp",
        "%{prj.name}/src/vendor/glm/glm/**.h",
		"%{prj.name}/src/vendor/glm/glm/**.inl"
	}

	includedirs
	{
        "%{prj.name}/src",
        "%{prj.name}/src/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"Dependencies/GLFW-3.3/lib-vc2019/Win64/glfw3.lib",
		"Dependencies/GLEW-2.1.0/lib/Win64/glew32s.lib",
		"opengl32.lib"
    }
    
    filter "files:Visualizer/src/vendor/imgui/*.cpp"
        flags { 'NoPCH' }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"GLEW_STATIC",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "On"

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
--                                                                                      --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------