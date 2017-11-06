project "scratch"
	language "C++"
	kind "ConsoleApp"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/scratch/")

	files {"include/**.h", "src/**.cpp"}

	includedirs {
		"include/",
		cpprelude_path .. "/include/"
	}

	links {"cpprelude"}

	filter "action:gmake"
		buildoptions {"-std=c++14", "-Wall", "-fno-rtti", "-fno-exceptions"}
		linkoptions {"-pthread"}

	filter "action:vs*"
		buildoptions {"-utf-8"}

	filter "configurations:debug"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"