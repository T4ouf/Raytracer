workspace "Raytracer"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "Raytracer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["freeglut"] = "Raytracer/src/lib/freeglut/"
IncludeDir["glm"] = "Raytracer/src/lib/glm/"
IncludeDir["lodePNG"] = "Raytracer/src/lib/lodePNG/"
IncludeDir["yaml"] = "Raytracer/src/lib/yaml/"

group ""

project "Raytracer"
	location "Raytracer"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.c"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.freeglut}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.lodePNG}",
		"%{IncludeDir.yaml}"
	}

	links{
		"Raytracer/src/lib/freeglut/freeglut.lib",
	}

	filter "system:windows"
		cppdialect "C++11"
		systemversion "latest"

		defines
		{
			
		}

		postbuildcommands
		{
			-- Copy latest binary to the "rendering" folder that contains all the scenes & .bat files
			("{COPY} %{cfg.buildtarget.relpath} %{prj.location}/rendering/ ") 
		}

	filter "configurations:Debug"
		defines {
			"RT_DEBUG",
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RT_RELEASE"
		runtime "Release"
		optimize "On"