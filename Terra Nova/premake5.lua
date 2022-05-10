 project "Terra Nova"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"

	targetdir ("%{wks.location}/bin/" .. outputdir.. "/%{prj.name}")
	objdir    ("%{wks.location}/bin-int/" .. outputdir.. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Terra Nova/Vendor/GLEW/include/",
		"%{wks.location}/Terra Nova/Vendor/GLFW/include/",
		"%{wks.location}/Terra Nova/Vendor",
		"Vendor/glm",

	}
	
	defines
	{
		"GLEW_STATIC",
	}

	libdirs 
	{
		"%{wks.location}/Terra Nova/Vendor/GLFW/lib-vc2022/",
		"%{wks.location}/Terra Nova/Vendor/GLEW/lib/Release/Win32/"
	}

	links
	{
		"glfw3.lib",
		"glew32s.lib",
		"opengl32.lib",
	}

	files
	{
		"Vendor/stb_image/**.h",
		"Vendor/stb_image/**.cpp",
		"Source/**.h",
		"Source/**.cpp",
		"Vendor/glm/glm/**.hpp",
		"Vendor/glm/glm/**.inl",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "off"

	filter "configurations:Debug"
		defines "DEBUG=1"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE=1"
		runtime "Release"
		optimize "on"

