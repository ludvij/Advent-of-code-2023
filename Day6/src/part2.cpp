#include <iostream>
#include <cmath>
#include "lud_utils.hpp"
#include "ctre.hpp"


int64_t do_races(const char* filename)
{
	Lud::Slurper file(filename);
	std::string times = "";
	std::string distances = "";
	for (const auto match : ctre::search_all<"(\\d+)">(file.ReadLine())) {
		times.append(match);
	}
	for (const auto match : ctre::search_all<"(\\d+)">(file.ReadLine())) {
		distances.append(match);
	}

	const int64_t t = Lud::parse_num<int64_t>(times);
	const int64_t d = Lud::parse_num<int64_t>(distances);

	const double inner  = static_cast<double>(sqrt(t*t - 4 * d));
	const int64_t begin = std::floor((static_cast<double>(t) - inner) * 0.5f + 1);
	const int64_t end   = std::ceil( (static_cast<double>(t) + inner) * 0.5f);

	return end - begin;
}

int main(int argc, char** argv) 
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}

	const int64_t res = do_races(argv[1]);

	std::cout << "Total: " << res << '\n';

	return 0;
}