#include <iostream>
#include <fstream>
#include <vector>
#include <list>


struct Lookup {
	const std::string_view str;
	const int32_t number;
};

std::vector<Lookup> lookups {
	{"1", 1},
	{"2", 2},
	{"3", 3},
	{"4", 4},
	{"5", 5},
	{"6", 6},
	{"7", 7},
	{"8", 8},
	{"9", 9},
};


uint32_t get_calibration(std::string filename)
{
	std::ifstream file(filename);

	uint32_t res = 0;

	for (std::string line; std::getline(file, line);) {
		uint32_t first_integer = 0;
		uint32_t first_integer_pos = UINT32_MAX;
		uint32_t second_integer = 0;
		uint32_t second_integer_pos = 0;

		for (const auto& lkup : lookups) {
			if (const size_t pos = line.find(lkup.str); pos != std::string::npos) {
				if (pos < first_integer_pos) {
					first_integer_pos = pos;
					first_integer = lkup.number;
				}
			}
			if (const size_t pos = line.rfind(lkup.str); pos != std::string::npos) {
				if (pos >= second_integer_pos) {
					second_integer_pos = pos;
					second_integer = lkup.number;
				}
			}
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