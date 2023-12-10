#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <numeric>
#include "lud_utils.hpp"

uint64_t do_maps(const char* filename) 
{
	std::unordered_map<std::string, std::tuple<std::string, std::string>> map;
	Lud::Slurper file(filename);
	const std::string sequence = file.ReadLine();
	std::vector<std::string> nodes_i_am_currently_on;
	for(const auto& line : file.ReadLines()) {
		if (line.empty()) continue;
		std::string curr  = line.substr(0, 3);
		std::string left  = line.substr(7, 3);
		std::string right = line.substr(12, 3);

		if (curr[2] == 'A') {
			nodes_i_am_currently_on.push_back(curr);
		}

		map.try_emplace(curr, std::make_tuple(left, right));
	}
	// through trial I dicovered that the range from **A to **Z is the same as **Z to **Z
	// so cycles without offsets
	std::vector<uint64_t> cycles;
	for (auto& n : nodes_i_am_currently_on) {
		auto next = n;
		uint32_t moves = 0;
		while(next[2] != 'Z') {
			const auto [l, r] = map[next];
			next = sequence[moves % sequence.length()] == 'L' ? l : r;
			moves++;
		}
		cycles.push_back(moves);
	}
	uint64_t res = 0;
	// do lcm
	for (size_t i = 1; i < cycles.size(); i++) {
		if (res == 0) {
			res = std::lcm(cycles[0], cycles[1]);
		}
		else {
			res = std::lcm(res, cycles[i]);
		}
	}
	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Uasage " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_maps(argv[1]);
	std::cout << "Total: " << res << '\n';
	return 0;
}