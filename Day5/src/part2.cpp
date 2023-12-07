#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
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

	bool rHasRange(const uint64_t destination) const 
	{
		return destination >= to && destination < (to + size);
	}

	uint64_t rConvert(const uint64_t destination) const 
	{
		return destination + converter;
	}

	uint64_t from;
	uint64_t to;
	uint64_t size;
	int64_t converter;
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
			std::stoull(source.str()), 
			std::stoull(destination.str()), 
			std::stoull(range.str())
		);
	}

	return mappers;
}


uint64_t do_seeds(const char* filename) 
{

	std::ifstream file(filename);
	std::vector<Mapper> seeds;
	std::string seeds_string;

	std::getline(file, seeds_string);

	for(const auto& match : ctre::search_all<"(\\d+) (\\d+)">(seeds_string)) {
		seeds.emplace_back(0, std::stoul(match.get<1>().str()), std::stoul(match.get<2>().str()));
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