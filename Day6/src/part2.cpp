#include <iostream>
#include <cmath>
#include "lud_utils.hpp"
#include "ctre.hpp"


uint64_t do_races(const char* filename)
{
	Lud::Slurper file(filename);
	std::string times = "";
	std::string distances = "";
	for (const auto match : ctre::search_all<"(\\d+)">(file.ReadLine().substr(11))) {
		times.append(match);
	}
	for (const auto match : ctre::search_all<"(\\d+)">(file.ReadLine().substr(11))) {
		distances.append(match);
	}

	int64_t t = Lud::parse_num<int64_t>(times);
	int64_t d = Lud::parse_num<int64_t>(distances);

	int64_t inner = t*t - 4 * d;
	int64_t begin = (static_cast<double>(t) - sqrt(static_cast<double>(inner))) * 0.5f;
	int64_t end   = (static_cast<double>(t) + sqrt(static_cast<double>(inner))) * 0.5f;

	// enforce inequality
	if (end == (t - begin)) end--;
	
	return end - begin;
}

int main(int argc, char** argv) 
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}

	const uint64_t res = do_races(argv[1]);

	std::cout << "Total: " << res << '\n';

	return 0;
}