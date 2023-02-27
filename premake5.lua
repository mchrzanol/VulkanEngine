workspace "VulkanEngine"
	architecture "x64"
	startproject "TestPlatform"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	} 

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["GLFW"] = "VulkanEngine/external/GLFW/include"
IncludeDir["Vulkan"] = "VulkanEngine/external/Vulkan/Include"
IncludeDir["imgui"] = "VulkanEngine/external/imgui"
IncludeDir["glm"] = "VulkanEngine/external/glm"

vulkanPath ="VulkanEngine/external/Vulkan"

group "external"
	include "VulkanEngine/external/GLFW"
	include "Vulkanengine/external/imgui"

group ""

project "VulkanEngine"
	location "VulkanEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir.. "/%{prj.name}/obj")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/src/main",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}"
	}

	libdirs { "VulkanEngine/external/Vulkan/Lib" }

	links
	{
		"GLFW",
		"ImGui",
		"VulkanEngine/external/Vulkan/Lib/vulkan-1.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		runtime "Debug"

		systemversion "latest"

		defines
		{
			"ENGINE_EXPORTS"
		}

		postbuildcommands
		{
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestPlatform")
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize	"On"

		
	project "TestPlatform"
		location "TestPlatform"
		kind "ConsoleApp"
		language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir.. "/%{prj.name}/obj")

		files
		{
			"%{prj.name}/**.h",
			"%{prj.name}/**.hpp",
			"%{prj.name}/**.cpp"
		}

		includedirs
		{
			"VulkanEngine/src",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Vulkan}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.imgui}"
		}

		links
		{
			"VulkanEngine"
		}

		filter "system:windows"
			cppdialect "C++20"
			staticruntime "On"
			systemversion "latest"

		defines
		{
				
		}

		filter "configurations:Debug"
			symbols "On"

		filter "configurations:Release"
			optimize "On"

		filter "configurations:Dist"
			optimize	"On"

