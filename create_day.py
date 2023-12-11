from sys import argv
import os
import os.path
import subprocess

premake_src = """workspace "AoC_2023_Day{day}"
	architecture "x86_64"
	language "C++"
	cppdialect "c++20"
	targetdir ("%%{{prj.location}}/bin")
	objdir ("%%{{prj.location}}/bin-int")

	flags {{
		"MultiProcessorCompile",
	}}

	buildoptions {{
		"-Wall", 
		"-Wextra", 
		"-Wshadow",
		"-pedantic", 
		"-Wnon-virtual-dtor"
	}}

	configurations {{
		"release",
		"debug"
	}}

	includedirs {{
		"src",
		"../vendor/include",
		"../utils"
	}}

	defines {{
		"LUD_SLURPER_IMPLEMENTATION"
	}}

	filter "configurations:debug"
	
		runtime "debug"
		symbols "On"

	filter "configurations:release"

		runtime "release"
		optimize "On"

project "Part1"
	kind "ConsoleApp"
	
	files {{ 
		"src/part1.cpp"
	}}

project "Part2"
	kind "ConsoleApp"

	files {{ 
		"src/part2.cpp"
	}}
"""

cpp_src = """// Aoc 2023 Day {day} Part {part}
// author: Lud
#include <iostream>
#include "lud_utils.hpp"

s64 do_operation(const char* filename)
{{
	Lud::Slurper file(filename);
	s64 res = 0;
	return res;
}}


int main(int argc, char** argv)
{{
	if (argc < 2) {{
		std::cout << "Usage: " << argv[0] << " path/to/file\\n";
		return 0;
	}}
	const auto res = do_operation(argv[1]);
	std::cout << "Total " << res << '\\n';
	return 0;
}}
"""


def main():
	if len(argv) == 1:
		print(f'Usage: {argv[0]} n')
		return
	day = int(argv[1])
	if not os.path.exists(f'./Day{day}'):
		print(f' creating folder: ./Day{day}/src')
		os.makedirs(f'./Day{day}/src')
		print('Creating premake file')
		with open(f'./Day{day}/premake5.lua', 'w') as f:
			f.write(premake_src.format(day=day))
		print('Creatiing source files')
		for i in range(2):
			with open(f'./Day{day}/src/part{i+1}.cpp', 'w') as f:
				f.write(cpp_src.format(day=day, part=i + 1))

		print('running premake build')
		os.chdir(f'./Day{day}')
		subprocess.call(['premake5', 'gmake2'])
		if os.name == 'posix':
			subprocess.call(['bear', '--', 'make'])
			subprocess.call(['make', 'clean'])







if __name__ == '__main__':
	main()