workspace "AoC_2023"
	architecture "x86_64"

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
		"Part1",
		"Part2",
	}



include "Day1"
include "Day2"
include "Day3"
include "Day4"
-- include "Day5"
