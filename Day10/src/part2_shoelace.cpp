// Aoc 2023 Day 10 Part 2 alternative solution
// author: Lud
#include "lud_assert.hpp"
#include <iostream>
#include <ostream>
#include <array>
#include <lud_utils.hpp>
#include <optional>
#include <vector>

using Point = Lud::Vec2<size_t>;

Point get_next(const std::vector<std::string>& lines, const Point& curr, const Point& prev)
{
	const auto [x, y] = curr;
	Point p1;
	Point p2;
	switch(lines[y][x]) {
		case '|': p1 = {x, y - 1}; p2 = {x, y + 1}; break;
		case '-': p1 = {x - 1, y}; p2 = {x + 1, y}; break;
		case 'L': p1 = {x, y - 1}; p2 = {x + 1, y}; break;
		case 'F': p1 = {x, y + 1}; p2 = {x + 1, y}; break;
		case 'J': p1 = {x, y - 1}; p2 = {x - 1, y}; break;
		case '7': p1 = {x, y + 1}; p2 = {x - 1, y}; break;
	}

	return p1 == prev ? p2 : p1;
}


Point interpret_s(std::vector<std::string>& lines, const Point& begin)
{
	const auto [x, y] = begin;
	std::optional<Point> p;
	int access = 0;
	if (y > 0 && (lines[y - 1][x] == '|' || lines[y - 1][x] == '7' || lines[y - 1][x] == 'F')) {
		p = {x, y-1};
		access |= 0b0001;
	}
	if (x > 0 && (lines[y][x - 1] == '-' || lines[y][x - 1] == 'L' || lines[y][x - 1] == 'F')) {
		if (!p) p = {x - 1, y};
		access |= 0b0010;
	}
	if (x < lines[y].size() && (lines[y][x + 1] == '-' || lines[y][x + 1] == 'J' || lines[y][x + 1] == '7')) {
		if (!p) p = {x + 1, y};
		access |= 0b0100;
	}
	if (y < lines.size() && (lines[y + 1][x] == '|' || lines[y + 1][x] == 'J' || lines[y + 1][x] == 'L')) {
		if (!p) p = {x, y + 1};
		access |= 0b1000;
	}
	if      (access == 0b0011) lines[y] = lines[y].replace(x, 1, "J");
	else if (access == 0b0101) lines[y] = lines[y].replace(x, 1, "L");
	else if (access == 0b1001) lines[y] = lines[y].replace(x, 1, "|");
	else if (access == 0b0110) lines[y] = lines[y].replace(x, 1, "-");
	else if (access == 0b1010) lines[y] = lines[y].replace(x, 1, "7");
	else if (access == 0b1100) lines[y] = lines[y].replace(x, 1, "F");
	return *p;
}

s64 do_operation(const char* filename)
{
	Lud::Slurper file(filename);
	auto lines = file.ReadLines();
	Point begin;
	for(size_t y = 0; y <lines.size(); y++) {
		if(size_t pos = lines[y].find('S'); pos != std::string::npos) {
			begin = {pos, y};
			break;
		}
	}

	Point interpreted = interpret_s(lines, begin);
	std::array<Point, 2> paths = {begin, interpreted};
	std::vector<Point> corners{};
	if (lines[begin.y][begin.x] != '-' && lines[begin.y][begin.x] != '|') {
		corners.push_back(begin);
	}
	double sz = 1;
	do {
		// get pos in maze
		Point next = get_next(lines, paths[1], paths[0]);
		paths[0] = paths[1];
		paths[1] = next;
		if (lines[paths[0].y][paths[0].x] != '-' && lines[paths[0].y][paths[0].x] != '|') {
			corners.push_back(paths[0]);
		}
		sz++;
	} while(paths[1] != begin);
	corners.push_back(corners[0]);
	double area = 0;
	// apply shoelace
	// https://en.wikipedia.org/wiki/Shoelace_formula
	for(size_t i = 0; i < corners.size() - 1; i++) {
		area += corners[i].x * corners[i + 1].y;
		area -= corners[i].y * corners[i + 1].x;
	}
	area = std::abs(area * .5f);
	// Pick's formula
	// https://en.wikipedia.org/wiki/Pick's_theorem
	return area - sz/2 + 1;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_operation(argv[1]);

	Lud::assert_eq(do_operation("test21.txt"), 8);
	Lud::assert_eq(do_operation("test22.txt"), 10);

	std::cout << "Total " << res << '\n';
	return 0;
}
