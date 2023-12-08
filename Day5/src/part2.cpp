#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include "ctre.hpp"
#include "lud_utils.hpp"


class Mapper {
public:

	bool rHasRange(const uint64_t destination) const 
	{
		return destination >= to && destination < (to + size);
	}

	uint64_t rConvert(const uint64_t destination) const 
	{
		return destination + from - to;
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
	std::vector<Mapper> seeds;

	std::string seeds_line = file.ReadLine();

	for(const auto& match : ctre::search_all<"(\\d+) (\\d+)">(seeds_line)) {
		const auto [_, begin, size] = match;
		seeds.emplace_back(
			0, 
			Lud::parse_num<uint64_t>(begin), 
			Lud::parse_num<uint64_t>(size)
		);
	}

	const auto mappers = make_mappers(file);

	uint64_t res = 0;
	while(1) {
		size_t start = res;
		for(auto it = mappers.rbegin(); it != mappers.rend(); ++it) {
			for (const auto& mapper : *it) {
				if (mapper.rHasRange(start)) {
					start = mapper.rConvert(start);
					break;
				}
			}
		}

		for (const auto& val: seeds) {
			if (val.rHasRange(start)) {
				return res;
			}
		}
		res++;
	}
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