project "cpprelude"
	language "C++"
	kind "SharedLib"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/cpprelude/")

	files {"include/**.h", "src/**.cpp"}

	includedirs {"include/"}

	if os.istarget("linux") then

		buildoptions {"-std=c++14", "-Wall", "-fno-rtti", "-fno-exceptions"}
		linkoptions {"-pthread"}

	elseif os.istarget("windows") then

		if os.getversion().majorversion == 10.0 then
			systemversion(win10_sdk_version())
		end

		buildoptions {"/utf-8"}

		filter "action:vs*"
			files {"tools/vs/cpprelude.natvis"}
	end

	filter "configurations:debug"
		targetsuffix "d"
		defines {"DEBUG", "COMPILE_DYNAMIC_LIB"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG", "COMPILE_DYNAMIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"