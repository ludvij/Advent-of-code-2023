#include <fstream>
#include <iostream>
#include "ctre.hpp"


int32_t do_cubes(const char* filename, const int32_t red, const int32_t green, const int32_t blue)
{
	std::ifstream file(filename);
	int32_t res = 0;
	for(std::string line; std::getline(file, line);) {

		if (const auto [whole_line, game, sets] = ctre::match<"Game (\\d+): (.+)">(line); whole_line) {
			uint32_t game_id = std::stoi(game.str());
			bool valid = true;

			for(const auto set : ctre::split<"; ">(sets)) {

				for (const auto cubes : ctre::split<", ">(set)) {

					if (const auto [whole_cube, sNumber, color] = ctre::match<"(\\d+) (red|green|blue)">(cubes); whole_cube) {

						const int32_t number = std::stoi(sNumber.str());
						if (color == "green" && number > green) {
							valid = false;
							break;
						}
						else if (color == "red" && number > red) {
							valid = false;
							break;
						}
						else if (color == "blue" && number > blue) {
							valid = false;
							break;
						}
					}
				}					
				if (!valid) break;
			}
			if (valid) {
				res += game_id;
			}
		}
	}

	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2 ) {
		std::cout << "Usage " << argv[0] << " path/to/file\n";
		return 0;
	}
	const uint32_t res = do_cubes(argv[1], 12, 13, 14);
	std::cout << res << '\n';
	return 0;
}
