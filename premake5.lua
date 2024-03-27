------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
-- The Visualizer Solution                                                              --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
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
IncludeDir["GLFW"]          = "Visualizer/vendor/glfw/include"
IncludeDir["GLEW"]          = "Visualizer/vendor/glew-2.2.0/include"
IncludeDir["ImGui"]         = "Visualizer/vendor/imgui"
IncludeDir["glm"]           = "Visualizer/vendor/glm"
IncludeDir["stb"]           = "Visualizer/vendor/stb"
IncludeDir["assimp"]        = "Visualizer/vendor/assimp/include"
IncludeDir["assimp_config"] = "Visualizer/vendor/assimp/build/include"

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
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    pchheader "PreCompiledHeader.h"
    pchsource "Visualizer/src/PreCompiledHeader.cpp"

    postbuildcommands { "{COPYDIR} %[Visualizer/resources] %[bin/" .. outputdir .. "/%{prj.name}/resources]" }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.h",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLEW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.assimp_config}"
    }

    links 
    { 
        "GLFW",
        "ImGui",
        "opengl32.lib",
        "Visualizer/vendor/glew-2.2.0/lib/Win64/glew32s.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GLEW_STATIC",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        defines
        {
            "DEBUG"
        }
        links
        {
            "Visualizer/vendor/assimp/build/lib/Debug/assimp-vc143-mtd.lib",
            "Visualizer/vendor/assimp/build/contrib/zlib/Debug/zlibstaticd.lib"
        }

    filter "configurations:Release"
        kind "WindowedApp"
        runtime "Release"
        optimize "on"
        symbols "off"
        links
        {
            "Visualizer/vendor/assimp/build/lib/Release/assimp-vc143-mt.lib",
            "Visualizer/vendor/assimp/build/contrib/zlib/Release/zlibstatic.lib"
        }

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
-- GLFW                                                                                 --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
project "GLFW"
    location "Visualizer"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{VendorDir.GLFW}/include/GLFW/glfw3.h",
        "%{VendorDir.GLFW}/include/GLFW/glfw3native.h",
        "%{VendorDir.GLFW}/src/context.c",
        "%{VendorDir.GLFW}/src/init.c",
        "%{VendorDir.GLFW}/src/input.c",
        "%{VendorDir.GLFW}/src/monitor.c",
        "%{VendorDir.GLFW}/src/null_init.c",
        "%{VendorDir.GLFW}/src/null_joystick.c",
        "%{VendorDir.GLFW}/src/null_monitor.c",
        "%{VendorDir.GLFW}/src/null_window.c",
        "%{VendorDir.GLFW}/src/platform.c",
        "%{VendorDir.GLFW}/src/vulkan.c",
        "%{VendorDir.GLFW}/src/window.c"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        files
        {
            "%{VendorDir.GLFW}/src/win32_init.c",
            "%{VendorDir.GLFW}/src/win32_joystick.c",
            "%{VendorDir.GLFW}/src/win32_module.c",
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
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        symbols "off"

------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
-- ImGui                                                                                --
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
project "ImGui"
    location "Visualizer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
        
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{VendorDir.ImGui}/*.h",
        "%{VendorDir.ImGui}/*.cpp",
    }
    
    filter "system:windows"
        systemversion "latest"

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        symbols "off"