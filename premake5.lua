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
IncludeDir["GLFW"] = "Visualizer/vendor/glfw/include"
IncludeDir["GLEW"] = "Dependencies/GLEW-2.1.0/include"
IncludeDir["ImGui"] = "Visualizer/vendor/imgui"

VendorDir = {}
VendorDir["GLFW"] = "Visualizer/vendor/glfw"
VendorDir["ImGui"] = IncludeDir["ImGui"]

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
        "GLFW",
        "ImGui",
		"Dependencies/GLEW-2.1.0/lib/Win64/glew32s.lib",
		"opengl32.lib"
    }

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
-- GLFW                                                                                 --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
project "GLFW"
    kind "StaticLib"
    language "C"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "%{VendorDir.GLFW}/include/GLFW/glfw3.h",
        "%{VendorDir.GLFW}/include/GLFW/glfw3native.h",
        "%{VendorDir.GLFW}/src/glfw_config.h",
        "%{VendorDir.GLFW}/src/context.c",
        "%{VendorDir.GLFW}/src/init.c",
        "%{VendorDir.GLFW}/src/input.c",
        "%{VendorDir.GLFW}/src/monitor.c",
        "%{VendorDir.GLFW}/src/vulkan.c",
        "%{VendorDir.GLFW}/src/window.c"
    }
    
	filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        
        files
        {
            "%{VendorDir.GLFW}/src/win32_init.c",
            "%{VendorDir.GLFW}/src/win32_joystick.c",
            "%{VendorDir.GLFW}/src/win32_monitor.c",
            "%{VendorDir.GLFW}/src/win32_time.c",
            "%{VendorDir.GLFW}/src/win32_thread.c",
            "%{VendorDir.GLFW}/src/win32_window.c",
            "%{VendorDir.GLFW}/src/wgl_context.c",
            "%{VendorDir.GLFW}/src/egl_context.c",
            "%{VendorDir.GLFW}/src/osmesa_context.c"
        }

		defines 
		{ 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}
    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
-- ImGui                                                                                --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
        project "ImGui"
        kind "StaticLib"
        language "C++"
        
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "%{VendorDir.ImGui}/imconfig.h",
            "%{VendorDir.ImGui}/imgui.h",
            "%{VendorDir.ImGui}/imgui.cpp",
            "%{VendorDir.ImGui}/imgui_draw.cpp",
            "%{VendorDir.ImGui}/imgui_internal.h",
            "%{VendorDir.ImGui}/imgui_widgets.cpp",
            "%{VendorDir.ImGui}/imstb_rectpack.h",
            "%{VendorDir.ImGui}/imstb_textedit.h",
            "%{VendorDir.ImGui}/imstb_truetype.h",
            "%{VendorDir.ImGui}/imgui_demo.cpp"
        }
        
        filter "system:windows"
            systemversion "latest"
            cppdialect "C++17"
            staticruntime "On"
            
        filter { "system:windows", "configurations:Release" }
            buildoptions "/MT"