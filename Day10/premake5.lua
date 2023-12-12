workspace "AoC_2023_Day10"
	architecture "x86_64"
	language "C++"
	cppdialect "c++20"
	targetdir ("%%{prj.location}/bin")
	objdir ("%%{prj.location}/bin-int")

	flags {
		"MultiProcessorCompile",
	}

	buildoptions {
		"-Wall", 
		"-Wextra", 
		"-Wshadow",
		"-pedantic", 
		"-Wnon-virtual-dtor"
	}

	configurations {
		"release",
		"debug"
	}

	includedirs {
		"src",
		"../vendor/include",
		"../utils"
	}

	defines {
		"LUD_SLURPER_IMPLEMENTATION"
	}

	filter "configurations:debug"
	
		runtime "debug"
		symbols "On"

	filter "configurations:release"

		runtime "release"
		optimize "On"

project "Part1"
	kind "ConsoleApp"
	
	files { 
		"src/part1.cpp"
	}

project "Part2"
	kind "ConsoleApp"

	files { 
		"src/part2.cpp"
	}

project "Part2_shoelace"
	kind "ConsoleApp"

	files { 
		"src/part2_shoelace.cpp"
	}