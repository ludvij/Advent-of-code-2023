#include <fstream>
#include <iostream>
#include "ctre.hpp"


int32_t do_cubes(const char* filename)
{
	std::ifstream file(filename);
	int32_t res = 0;
	for(std::string line; std::getline(file, line);) {
		int32_t max_red = 0;
		int32_t max_green = 0;
		int32_t max_blue = 0;
		
		for (auto [whole, amount, color] : ctre::search_all<"(\\d+) (red|green|blue)">(line)) {
			int32_t number = std::stoi(amount.str());
			if (color == "red" && max_red < number) {
				max_red = number;
			}
			else if (color == "green" && max_green < number) {
				max_green = number;
			}
			else if (color == "blue" && max_blue < number) {
				max_blue = number;
			}
		}

		res += max_red * max_green * max_blue;
	}

	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2 ) {
		std::cout << "Usage " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint32_t res = do_cubes(argv[1]);
	std::cout << res << '\n';
	return 0;
}
