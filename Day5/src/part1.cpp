#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <algorithm>
#include "ctre.hpp"


class Mapper {
public:
	Mapper(const uint64_t source, const uint64_t destination, const uint64_t range)
		: from(source)
		, to(destination)
		, size(range)
	{
		converter = source - destination;
	}

	bool HasRange(const uint64_t source) const
	{
		return source >= from && source < (from + size);
	}

	uint64_t Convert(const uint64_t source) const
	{
		return source - converter;
	}

	int64_t converter;
	uint64_t from;
	uint64_t to;
	uint64_t size;
};


std::vector<std::vector<Mapper>> make_mappers(std::ifstream& file)
{
	std::vector<std::vector<Mapper>> mappers;
	for(std::string line; std::getline(file, line);) {
		if (line.empty()) {
			continue;
		}
		else if (ctre::match<"\\w+-to-\\w+ map:">(line)) {
			mappers.emplace_back();
			continue;
		}
		const auto [match, destination, source, range] = ctre::match<"(\\d+) (\\d+) (\\d+)">(line);
		mappers.back().emplace_back(
			std::stoul(source.str()), 
			std::stoul(destination.str()), 
			std::stoul(range.str())
		);
	}

	return mappers;
}


uint64_t do_seeds(const char* filename) 
{
	std::ifstream file(filename);
	std::vector<uint64_t> seeds;
	std::string seeds_line;
	std::getline(file, seeds_line);
	for(const auto match : ctre::search_all<"(\\d+)">(seeds_line)) {
		seeds.push_back(std::stoul(match.str()));
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