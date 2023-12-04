// alternate solution
// it's slower than the original
// but I wanted to try doing it this way

#include <iostream>
#include <list>
#include <vector>
#include <fstream>

struct Lookup {
	const std::string_view str;
	const int32_t number;
};

std::vector<Lookup> lookups {
	{"1", 1}, {"one",   1},
	{"2", 2}, {"two",   2},
	{"3", 3}, {"three", 3},
	{"4", 4}, {"four",  4},
	{"5", 5}, {"five",  5},
	{"6", 6}, {"six",   6},
	{"7", 7}, {"seven", 7},
	{"8", 8}, {"eight", 8},
	{"9", 9}, {"nine",  9},
};

struct LookupCandidate { 
	const Lookup* lkp; 
	int32_t pos; 
};


uint32_t get_calibration(std::string filename) 
{
	std::ifstream file(filename);

	uint32_t res = 0;

	for (std::string line; std::getline(file, line);) {
		std::list<LookupCandidate> candidates;
		uint32_t first_integer = -1;
		uint32_t second_integer = -1;

		for (size_t index = 0; index < line.length(); index++) {
			const char c = line[index];
			// add candidates
			for (const auto& lkup : lookups) {
				if (c == lkup.str[0] && lkup.str.length() <= (line.length() - index)) {
					candidates.push_back({&lkup, 0});
				}
			}
			// evaluate candidates
			for (auto& candidate : candidates) {
				
				if (candidate.lkp->str[candidate.pos++] != c) {
					candidate.pos = -1;
				}
				else {
					if (candidate.pos == candidate.lkp->str.length()) {
						if (first_integer == -1) {
							first_integer = candidate.lkp->number;
						}
						second_integer = candidate.lkp->number;
						candidate.pos = -1;
					}			
				}
			}
			// remove unsuited candidates
			candidates.remove_if([](auto x){return x.pos == -1; });
		}
		res += first_integer * 10 + second_integer;
	}

	return res;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << "path/to/file\n";
		return 0;
	}
	const uint32_t res = get_calibration(argv[1]);
	std::cout << "Total: " << res << '\n';
	
	return 0;
}