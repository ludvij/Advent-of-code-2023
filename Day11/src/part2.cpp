// Aoc 2023 Day 11 Part 2
// author: Lud
#include <iostream>
#include <lud_utils.hpp>

s64 do_operation(const char* filename)
{
	Lud::Slurper file(filename);
	s64 res = 0;
	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_operation(argv[1]);
	std::cout << "Total " << res << '\n';
	return 0;
}
