workspace "AoC_2023_Day4"
	architecture "x86_64"
	language "C++"
	cppdialect "c++20"
	targetdir ("%{prj.location}/bin")
	objdir ("%{prj.location}/bin-int")

	flags {
		"MultiProcessorCompile",
	}

	buildoptions {
		"-Wall", 
		"-Wextra", 
		"-pedantic", 
		"-Wshadow",
		"-Wnon-virtual-dtor"
	}

	configurations {
		"debug", 
		"release"
	}

	includedirs {
		"src",
		"../vendor/include"
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
	
