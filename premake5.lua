workspace "ShaderTool"
	configurations { "Debug", "Release" }
	platforms "x64"
	startproject "ShaderTool"

	project "glfw"
		location "vendor/glfw"
		kind "StaticLib"
		language "C"
		cdialect "C17"
		targetdir "bin/%{prj.name}"
		objdir "obj/%{prj.name}"
		files {
			"vendor/glfw/src/**.c",
			"vendor/glfw/include/**.h"
		}
		vpaths {
			["Source"] = "**.c",
			["Headers"] = "**.h"
		}
		filter "system:windows"
			defines {
				"_GLFW_WIN32",
				"_CRT_SECURE_NO_WARNINGS"
			}
		filter "configurations:Debug"
			defines "DEBUG"
			symbols "On"
		filter "configurations:Release"
			defines "NDEBUG"
			optimize "On"
	
	project "glad"
		location "vendor/glad"
		kind "StaticLib"
		language "C"
		cdialect "C17"
		targetdir "bin/%{prj.name}"
		objdir "obj/%{prj.name}"
		includedirs {
			"vendor/glad/include"
		}
		files {
			"vendor/glad/**.c",
			"vendor/glad/**.h"
		}
		vpaths {
			["Source"] = "**.c",
			["Headers"] = "**.h"
		}
		filter "configurations:Debug"
			defines "DEBUG"
			symbols "On"
		filter "configurations:Release"
			defines "NDEBUG"
			optimize "On"

	project "ShaderTool"
		kind "ConsoleApp"
		location "ShaderTool"
		language "C++"
		cppdialect "C++17"
		targetdir "bin/%{prj.name}"
		objdir "obj/%{prj.name}"
		files {
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/res/**.glsl"
		}
		vpaths {
			["Source"] = "**.cpp",
			["Headers"] = "**.hpp",
			["Resource"] = "**.glsl"
		}
		includedirs {
			"%{prj.name}/src",
			"vendor/glfw/include",
			"vendor/glad/include",
			"vendor/glm"
		}
		links {
			"glfw",
			"glad",
		}
		filter "configurations:Debug"
			defines "DEBUG"
			symbols "On"
		filter "configurations:Release"
			defines "NDEBUG"
			optimize "On"
