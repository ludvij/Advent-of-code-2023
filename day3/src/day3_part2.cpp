#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <deque>


struct Cell {
	int32_t value = 0;
	int32_t pos = 0;
	int32_t size = 0;
	bool found = false;

	enum class Type {
		Symbol,
		Number,
		Empty,
	} type = Type::Empty;
};


std::vector<Cell> make_cells(std::string line)
{
	Cell candidate;
	std::vector<Cell> current;
	current.resize(line.length());
	for (size_t i = 0; i < line.length(); i++) {
		const char c = line[i];
		if (std::isdigit(c)) {
			if (candidate.type == Cell::Type::Empty) {
				candidate.pos = i;
				candidate.type = Cell::Type::Number;
			}
			candidate.value = candidate.value * 10 + (c - 0x30);
			candidate.size++;
		}
		else {
			if (candidate.type != Cell::Type::Empty) {
				current[candidate.pos] = std::move(candidate);
				candidate = Cell();
			}
			if (c == '*') {
				current[i] = {0, static_cast<int32_t>(i), 1, false, Cell::Type::Symbol};
			}
		}
	}
	if (candidate.type != Cell::Type::Empty) {
		current[candidate.pos] = std::move(candidate);
	}

	return current;
}


uint32_t cross_compare(std::vector<Cell>& v1, const std::vector<Cell>& v2)
{
	uint32_t res = 0;
	for (size_t i = 0; i < v1.size(); i++) {
		auto& cell = v1[i];
		if (!(cell.type == Cell::Type::Number && !cell.found)) {
			continue;
		}
			
		size_t begin_x = i == 0 ? i : i - 1;
		size_t end_x = (i + cell.size) == v1.size() ? i + cell.size - 1: i + cell.size;
		if (v1[begin_x].type == Cell::Type::Symbol || v1[end_x].type == Cell::Type::Symbol) {
			res += cell.value;
			cell.found = true;
		}
		for (size_t j = begin_x; j <= end_x && j < v2.size(); j++) {
			if (!cell.found && v2[j].type == Cell::Type::Symbol) {
				res += cell.value;
				cell.found = true;
			} 
		}
		if (cell.found) {
			std::cout << "  [FOUND] " <<cell.value << " ["<<begin_x <<", "<< end_x << "]" <<'\n';
		}
	}
	return res;
}


uint32_t find_parts(const char* filename) 
{
	std::ifstream file(filename);
	uint32_t res = 0;

	std::deque<std::vector<Cell>> lines;
	std::string past_line;
	for(std::string line; getline(file, line);) {
		lines.push_back(make_cells(line));
		if (lines.size() > 2) {
			lines.pop_front();
		}
		std::cout << "Comparing: " << past_line << '\n';
		std::cout << "Against:   " << line << '\n';
		res += cross_compare(lines.front(), lines.back());

		std::cout << "Comparing: " << line << '\n';
		std::cout << "Against:   " << past_line << '\n';
		res += cross_compare(lines.back(), lines.front());
	
	}
	return res;
}


int main(int argc, char** argv) 
{
	if (argc < 2) {
		std::cout << "Usage " << argv[0] << "path/to/file\n";
		return 0;
	}
	uint32_t res = find_parts(argv[1]);
	std::cout << "Total: " << res << '\n';
	return 0;
}