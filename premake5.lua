workspace "NESEmu"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NESEmu"
	location "NESEmu"
	kind "ConsoleApp"
	language "C++"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}
		
	filter "configurations:Debug"
		symbols "On"
	
	filter "configurations:Release"
		optimize "On"
