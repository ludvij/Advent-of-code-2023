#include <iostream>
#include <unordered_map>
#include <tuple>
#include "lud_utils.hpp"

uint64_t do_maps(const char* filename) 
{
	std::unordered_map<std::string, std::tuple<std::string, std::string>> map;
	Lud::Slurper file(filename);
	const std::string sequence = file.ReadLine();
	for(const auto& line : file.ReadLines()) {
		if (line.empty()) continue;
		std::string curr  = line.substr(0, 3);
		std::string left  = line.substr(7, 3);
		std::string right = line.substr(12, 3);

		map.try_emplace(curr, std::make_tuple(left, right));
	}
	int64_t moves = 0;

	std::string next = "AAA";
	while(1) {
		const auto [l, r] = map[next];
		if (sequence[moves % sequence.length()] == 'L') {
			next = l;
		}
		else {
			next = r;
		}
		moves++;
		if (next == "ZZZ") break;
	}
	return moves;
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