#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include "ctre.hpp"

uint32_t do_cards(const char* filename) {
	std::ifstream file(filename);
	uint32_t res = 0;
	for(std::string line; std::getline(file, line);) {
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
		res += count;
	}
	return res;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
	}
	const uint32_t res = do_cards(argv[1]);
	std::cout << "Total: " << res << '\n';
}