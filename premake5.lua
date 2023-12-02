workspace "ConstellationEngine"
	architecture "x64"

	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ConstellationEngineCore"
	location "%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	files
	{
		"ConstellationEngineCore/include/**.h",
		"ConstellationEngineCore/src/**.cpp"
	}

	-- Include directories relative to solutiondir

	-- include "vendor/SFML/premake5.lua"
	-- include "vendor/imgui/premake5.lua"
	-- include "vendor/imgui-sfml/premake5.lua"
	-- include "vendor/ImGuiFileDialog/premake5.lua"
	-- include "vendor/yaml-cpp/premake5.lua"

	IncludeDir = {}
	-- IncludeDir["yaml-cpp"] = "vendor/yaml-cpp/include"

	includedirs
	{
		"ConstellationEngineCore/include"
		-- "vendor/imgui",
		-- "vendor/imgui/misc/cpp",
		-- "vendor/ImGuiFileDialog",
		-- "vendor/yaml-cpp/include"
	}

	links
	{
		-- "yaml-cpp",
		-- "ImGui",
		-- "ImGuiFileDialog"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"