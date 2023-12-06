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


class Mapper_set {
public:

	void AppendMap(const uint64_t source, const uint64_t destination, const uint64_t range)
	{
		mappers.emplace_back(source, destination, range);
	}

	uint64_t Convert(const uint64_t source) const
	{
		for(const auto& mapper: mappers) {
			if (mapper.HasRange(source)) {
				return mapper.Convert(source);
			}
		}

		return source;
	}

	std::vector<Mapper> mappers;
};




std::vector<Mapper_set> make_mappers(std::ifstream& file)
{
	std::vector<Mapper_set> mappers;
	for(std::string line; std::getline(file, line);) {
		if (line.empty()) {
			continue;
		}
		else if (const auto [match, from, to] = ctre::match<"(\\w+)-to-(\\w+) map:">(line); match) {
			mappers.emplace_back();
			continue;
		}
		const auto [match, destination, source, range] = ctre::match<"(\\d+) (\\d+) (\\d+)">(line);
		mappers.back().AppendMap(
			std::stoull(source.str()), 
			std::stoull(destination.str()), 
			std::stoull(range.str())
		);
	}

	return mappers;
}


uint32_t do_seeds(const char* filename) 
{
	std::ifstream file(filename);
	std::vector<uint64_t> seeds;
	std::string seeds_string;
	std::getline(file, seeds_string);
	for(const auto match : ctre::search_all<"(\\d+)">(seeds_string)) {
		std::cout << match << '\n';
		seeds.push_back(std::stoull(match.str()));
	}

	const auto mappers = make_mappers(file);

	uint64_t res = UINT32_MAX;

	for(auto& seed : seeds) {
		uint64_t current = seed;
		for (const auto& mapper : mappers) {
			const auto next = mapper.Convert(current);
			std::cout << " " << current << " " << next;
			current = next;
		}
		if (current < res) {
			res = current;
		}
		std::cout << '\n';
	}

	return res;
}



int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint32_t res = do_seeds(argv[1]);
	std::cout << "Total: " << res << '\n';
}