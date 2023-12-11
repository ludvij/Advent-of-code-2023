#include <iostream>
#include <cmath>
#include "lud_utils.hpp"
#include "ctre.hpp"



std::vector<std::vector<s64>> get_diffs(const std::vector<s64>& line)
{
	std::vector<std::vector<s64>> diffs;

	diffs.emplace_back(line);

	size_t zeros = 0;
	while(zeros != diffs.back().size()) {
		zeros = 0;
		diffs.emplace_back();
		diffs.back().resize(diffs[diffs.size() - 2].size() - 1);
		for(size_t i = 1; i < diffs[diffs.size() - 2].size(); i++) {
			diffs.back()[i - 1] = diffs[diffs.size() - 2][i] - diffs[diffs.size() - 2][i - 1];
			if (diffs.back()[i - 1] == 0) zeros++;
		}
	}

	return diffs;
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
	
	for(const auto& val: vals) {
		auto diffs = get_diffs(val);
		diffs.back().push_back(0);

		std::cout << '\n';
		for(size_t i = 1; i < diffs.size(); i++) {
			const size_t index = diffs.size() - 1 - i;
			const auto& last = diffs[index + 1];
			auto& curr = diffs[index];
			// only change lol
			curr.push_back(curr[0] - last.back());
		}
		res += diffs[0].back();
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
