project "Day5"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++20"
	targetdir ("%{prj.location}/bin")
	objdir ("%{prj.location}/bin-int")

	
	includedirs {
		"src",
		"../vendor/include"
	}
	
	
	filter "configurations:Part1"
	
		runtime "debug"
		symbols "On"
		files { 
			"src/**.hpp", 
			"src/part1.cpp"
		}

	filter "configurations:Part2"

		runtime "debug"
		symbols "On"
		files { 
			"src/**.hpp", 
			"src/part2.cpp"
		}


