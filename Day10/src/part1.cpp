// Aoc 2023 Day 10 Part 1
// author: Lud
#include "lud_assert.hpp"
#include <iostream>
#include <ostream>
#include <array>
#include <lud_utils.hpp>
#include <optional>

struct Point {
	size_t x;
	size_t y;
	bool operator != (const Point& other) const {
		return !(*this == other);
	}
	bool operator == (const Point& other) const {
		return x == other.x && y == other.y;
	}
};


Point get_begin(const std::vector<std::string>& lines)
{
	for(size_t y = 0; y <lines.size(); y++) {
		for(size_t x = 0; x < lines[y].size(); x++) {
			if (lines[y][x] == 'S') {
				return Point(x, y);
			}
		}
	}
	return {0, 0};
}



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


std::array<Point, 2> interpret_s(const std::vector<std::string>& lines, const Point& begin)
{
	const auto [x, y] = begin;
	std::optional<Point> p1;
	std::optional<Point> p2;
	if (y != 0 && (lines[y - 1][x] == '|' || lines[y - 1][x] == '7' || lines[y - 1][x] == 'F')) {
		p1 = {x, y-1};
	}
	if (x != 0 && (lines[y][x - 1] == '-' || lines[y][x - 1] == 'L' || lines[y][x - 1] == 'F')) {
		if      (!p1) p1 = {x - 1, y};
		else if (!p2) p2 = {x - 1, y};
	}
	if (x < lines[y].size() && (lines[y][x + 1] == '-' || lines[y][x + 1] == 'J' || lines[y][x + 1] == '7')) {
		if      (!p1) p1 = {x + 1, y};
		else if (!p2) p2 = {x + 1, y};
	}
	if (y < lines.size() && (lines[y + 1][x] == '|' || lines[y + 1][x] == 'J' || lines[y + 1][x] == 'L')) {
		p2 = {x, y + 1};
	}
	return {*p1, *p2};
}


s64 do_operation(const char* filename)
{
	Lud::Slurper file(filename);
	auto lines = file.ReadLines();
	Point begin = get_begin(lines);
	// 0 1 left
	// 2 3 right
	auto interpreted = interpret_s(lines, begin);
	std::array<Point, 4> paths = {begin, interpreted[0], begin, interpreted[1]};
	s64 res = 1;
	do {
		for (size_t i = 1; i < 4; i+= 2) {
			// get pos in maze
			Point next = get_next(lines, paths[i], paths[i - 1]);
			paths[i - 1] = paths[i];
			paths[i] = next;
		}
		res++;
	} while(paths[1] != paths[3]);

	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_operation(argv[1]);

	Lud::assert_eq(do_operation("test11.txt"), 4);
	Lud::assert_eq(do_operation("test12.txt"), 8);

	std::cout << "Total " << res << '\n';
	return 0;
}
