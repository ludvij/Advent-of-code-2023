#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <algorithm>
#include "ctre.hpp"
#include "lud_utils.hpp"


class Mapper {
public:

	bool HasRange(const uint64_t source) const
	{
		return source >= from && source < (from + size);
	}

	uint64_t Convert(const uint64_t source) const
	{
		return source - from + to;
	}

	uint64_t from;
	uint64_t to;
	uint64_t size;
};


std::vector<std::vector<Mapper>> make_mappers(Lud::Slurper& file)
{
	std::vector<std::vector<Mapper>> mappers;
	for(const auto& line : file.ReadLines()) {
		if (line.empty()) {
			continue;
		}
		else if (ctre::match<"\\w+-to-\\w+ map:">(line)) {
			mappers.emplace_back();
			continue;
		}
		const auto [match, destination, source, range] = ctre::match<"(\\d+) (\\d+) (\\d+)">(line);
		mappers.back().emplace_back(
			Lud::parse_num<uint64_t>(source), 
			Lud::parse_num<uint64_t>(destination), 
			Lud::parse_num<uint64_t>(range)
		);
	}

	return mappers;
}


uint64_t do_seeds(const char* filename) 
{
	Lud::Slurper file(filename);
	std::vector<uint64_t> seeds;
	std::string seeds_line = file.ReadLine();
	for(const auto match : ctre::search_all<"(\\d+)">(seeds_line)) {
		seeds.push_back(Lud::parse_num<uint64_t>(match));
	}

	const auto mappers = make_mappers(file);

	uint64_t res = UINT64_MAX;
	for(auto& seed : seeds) {
		uint64_t current = seed;
		for (const auto& ms : mappers) {
			for (const auto& m : ms) {
				if (m.HasRange(current)) {
					current = m.Convert(current);
					break;
				}
			}
		}
		if (res > current) {
			res = current;
		}
	}
	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint64_t res = do_seeds(argv[1]);
	std::cout << "Total: " << res << '\n';
}