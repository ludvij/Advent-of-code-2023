#include <iostream>
#include <numeric>
#include "lud_utils.hpp"
#include "ctre.hpp"



s64 get_diffs(std::vector<s64>& line)
{
	size_t zeros = 0;
	size_t curr = line.size();
	while(curr != zeros) {
		zeros = 0;
		for(size_t i = 0; i < curr - 1; i++) {
			line[i] = line[i + 1] - line[i];
			if (line[i] == 0) zeros++;
		}
		curr--;
	}

	return std::accumulate(line.begin(), line.end(), 0);
}

int do_mirage(const char* filename)
{
	s64 res = 0;
	Lud::Slurper file(filename);
	std::vector<std::vector<s64>> vals;
	for(const auto& line : file.ReadLines()) {
		vals.emplace_back();
		for(const auto& match : ctre::search_all<"(-?\\d+)">(line)) {
			vals.back().push_back(Lud::parse_num<s64>(match));
		}
	}
	
	for(auto& val: vals) {
		res += get_diffs(val);
	}
	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2 ) {
		std::cout << "Usage " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint32_t res = do_mirage(argv[1]);
	std::cout << "Total: " << res << '\n';
	return 0;
}
