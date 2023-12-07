workspace "ConstellationEngine"
	architecture "x64"

	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "ConstellationEngineCore/vendor/glfw/premake5.lua"

project "ConstellationEngineCore"
	location "%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"
	
	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	-- ignoredefaultlibraries { "libcmt" }
	
	files
	{
		"ConstellationEngineCore/include/**.h",
		"ConstellationEngineCore/include/**.hpp",
		"ConstellationEngineCore/src/**.cpp"
	}
	
	-- Include directories relative to solutiondir

	VULKAN_SDK = os.getenv("VULKAN_SDK")

	LibraryDir = {}
	LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
	LibraryDir["glfw"] = "ConstellationEngineCore/vendor/glfw/bin/Debug-windows-x86_64/glfw"
	
	Library = {}
	Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
	Library["glfw"] = "ConstellationEngineCore/vendor/glfw/bin/Debug-windows-x86_64/glfw/glfw.lib"
	-- Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

	Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
	Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
	Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
	Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

	Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
	Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
	Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

	
	includedirs
	{
		"ConstellationEngineCore/include",
		"%{VULKAN_SDK}/Include",
		"ConstellationEngineCore/vendor/glfw/include",
		"ConstellationEngineCore/vendor/glm"
		-- "vendor/imgui",
		-- "vendor/imgui/misc/cpp",
		-- "vendor/ImGuiFileDialog",
		-- "vendor/yaml-cpp/include"
	}

	libdirs
	{
		"%{LibraryDir.VulkanSDK}",
		"%{LibraryDir.glfw}"
	}

	
	links
	{
		-- "%{Library.VulkanUtils}",
		"vulkan-1.lib",
		"glfw.lib"
		-- "yaml-cpp",
		-- "ImGui",
		-- "ImGuiFileDialog"
	}
	
	-- filter { "system:windows" }
   	-- 	prebuildcommands { 	"@echo",
	-- 						"%{VULKAN_SDK}/Bin/glslc.exe $(SolutionDir)ConstellationEngineCore/shader/SimpleShader.vert -o $(SolutionDir)ConstellationEngineCore/shader/vert.spv",
	-- 	   				   	"%{VULKAN_SDK}/Bin/glslc.exe $(SolutionDir)ConstellationEngineCore/shader/SimpleShader.frag -o $(SolutionDir)ConstellationEngineCore/shader/frag.spv"}
	-- include "vendor/imgui/premake5.lua"
	-- include "vendor/imgui-sfml/premake5.lua"
	-- include "vendor/ImGuiFileDialog/premake5.lua"
	-- include "vendor/yaml-cpp/premake5.lua"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"