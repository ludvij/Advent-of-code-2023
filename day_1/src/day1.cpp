#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>


struct Lookup {
	std::string_view str_version;
	int32_t number_version;
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

uint32_t get_calibration(std::string filename)
{
	std::ifstream file(filename);
	std::string line;

	uint32_t res = 0;

	while (std::getline(file, line)) {
		uint32_t first_integer = 0;
		uint32_t first_integer_pos = UINT32_MAX;
		uint32_t second_integer = 0;
		uint32_t second_integer_pos = 0;

		for (const auto& lkup : lookups) {
			if (const size_t pos = line.find(lkup.str_version); pos != std::string::npos) {
				if (pos < first_integer_pos) {
					first_integer_pos = pos;
					first_integer = lkup.number_version;
				}
			}
			if (const size_t pos = line.rfind(lkup.str_version); pos != std::string::npos) {
				if (pos >= second_integer_pos) {
					second_integer_pos = pos;
					second_integer = lkup.number_version;
				}
			}
		}
		res += first_integer * 10 + second_integer;
		std::cout << line << '\n';
		std::cout << "  " << first_integer << second_integer << '\n';
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