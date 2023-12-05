#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include "ctre.hpp"


uint32_t calculate_next_cards(std::string line) 
{
	int32_t count = 0;
	std::unordered_set<std::string_view> winning_set;
	auto [whole, winning, have] = ctre::match<".+: (.+) \\| (.+)">(line);
	for (auto match : ctre::search_all<"(\\d+)">(winning)) {
		winning_set.insert(match.view());
	}
	for (auto match : ctre::search_all<"(\\d+)">(have)) {
		if (winning_set.contains(match.view())) {
			count++;
		}
	}
	return count;
}

uint32_t do_cards(std::vector<std::string> data) 
{
	uint32_t res = 0;
	for(size_t i = 0; i < data.size(); i++) {
		res += calculate_next_cards(data[i]);
	}

	return res;
}

std::vector<std::string> slurp(const char* filename)
{
	std::ifstream file(filename);
	std::vector<std::string> lines;
	for(std::string line; std::getline(file, line);) {
		lines.push_back(line);
	}

	return lines;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
	}
	auto data = slurp(argv[1]);
	const uint32_t res = do_cards(data);
	std::cout << "Total: " << res << '\n';
}