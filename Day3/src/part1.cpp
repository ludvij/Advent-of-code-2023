#include <iostream>
#include <fstream>
#include <string>
#include <vector>



uint32_t find_parts(const std::vector<std::string>& lines) 
{
	uint32_t res = 0;
	for(size_t y = 0; y < lines.size(); y++) {
		for(size_t x = 0; x < lines[y].length(); x++) {
			if (!std::isdigit(lines[y][x])) {
				continue;
			}
			int32_t number = 0;
			size_t x0 = x == 0 ? x : x - 1;
			size_t y0 = y == 0 ? y : y - 1;
			for (size_t found = x; found < lines[y].length(); found++) {
				if (!std::isdigit(lines[y][found])) {
					break;
				}
				number = number * 10 + (lines[y][found] - 0x30);
				x++;
			}
			size_t x1 = x == lines[y].size() ? x : x + 1;
			size_t y1 = (y + 1) == lines.size() ? y : y + 1;
			bool found = false;
			for (size_t fy = y0; fy <= y1; fy++) {
				for (size_t fx = x0; fx < x1; fx++) {
					if (lines[fy][fx] != '.' && !std::isdigit(lines[fy][fx])) {
						res += number;
						found = true;
					}
					if (found) break;
				}
				if (found) break;
			}
		}
	}
	return res;
}

std::vector<std::string> slurp(const char* filename) 
{
	std::ifstream file(filename);
	std::vector<std::string> lines;
	for(std::string line; std::getline(file, line);) {
		lines.push_back(std::move(line));
	}

	return lines;
}


int main(int argc, char** argv) 
{
	if (argc < 2) {
		std::cout << "Usage " << argv[0] << "path/to/file\n";
		return 0;
	}
	auto lines = slurp(argv[1]);
	const uint32_t res = find_parts(lines);
	std::cout << "Total: " << res << '\n';
	return 0;
}