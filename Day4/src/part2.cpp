#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <vector>
#include "ctre.hpp"


uint32_t calculate_wins(const std::string& line) 
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


uint32_t do_cards(const std::vector<std::string>& data) 
{
	uint32_t res = 0;
	std::vector<uint32_t> wins;
	wins.resize(data.size());

	for (size_t i = data.size(); i > 0; i--) {
		const uint32_t card_wins = calculate_wins(data[i - 1]);
		wins[i - 1] = card_wins;
		for(uint32_t j = 0; j < card_wins; j++) {
			if (i + j >= wins.size()) {
				break;
			}
			wins[i - 1] += wins[i + j];
		}
	}
	for (size_t i = 0; i < wins.size(); i++) {
		res += wins[i] + 1;
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