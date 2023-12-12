// Aoc 2023 Day 10 Part 1
// author: Lud
#include <iostream>
#include <ostream>
#include <array>
#include <lud_utils.hpp>
#include <optional>
#include <vector>
#include <algorithm>
#include <unordered_set>

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


template <> struct std::hash<Point> {
	size_t operator()(const Point& s) const noexcept {
		const size_t x = std::hash<size_t>{}(s.x);
		const size_t y = std::hash<size_t>{}(s.y);

		return (x * 73856093) ^ y;
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

void print_cycle(const std::unordered_set<Point>& cycle, const std::unordered_set<Point>& inside, const std::vector<std::string>& ref)
{
	std::stringstream ss;
	for(size_t i = 0; i < ref.size(); i++) {
		for(size_t j = 0; j < ref[i].size(); j++) {
			Point test = {j, i};
			if (!cycle.contains(test)) {
				if (inside.contains(test)) {
					ss << "█";
				}
				else {
					ss << ' ';
				}
				continue;
			}
			ss << "\x1b[38;5;2m";
			switch (ref[i][j]) {
				case 'F': ss << "┌"; break;
				case '7': ss << "┐"; break;
				case 'L': ss << "└"; break;
				case 'J': ss << "┘"; break;
				default : ss << ref[i][j];
			}
			ss << "\x1b[0m";
		}
		ss << '\n';
	}
	std::cout << ss.str();
}

bool check_inside(const Point& p, const std::unordered_set<Point>& cycle, const std::vector<std::string>& ref)
{
	int intersections = 0;
	for(size_t x = p.x; x < ref[p.y].size(); x++) {
		Point test = {x, p.y};
		if (!cycle.contains(test)) {
			continue;
		}
		const char c = ref[p.y][x];
		// we interpret the position as it was in a botom corner
		// so it can intersect with J or L since they go from mid to up
		if (c != '-' && c != 'J' && c != 'L') {
			intersections++;
		}
	}
	return intersections % 2 == 1 && intersections != 0;
}


s64 do_operation(const char* filename)
{
	Lud::Slurper file(filename);
	auto lines = file.ReadLines();
	Point begin = get_begin(lines);

	auto interpreted = interpret_s(lines, begin);
	std::array<Point, 2> paths = {begin, interpreted[0]};
	std::unordered_set<Point> cycle{begin};
	s64 res = 0;
	do {
		// get pos in maze
		Point next = get_next(lines, paths[1], paths[0]);
		paths[0] = paths[1];
		paths[1] = next;
		cycle.insert(paths[0]);
	} while(paths[1] != begin);

	std::unordered_set<Point> inside;
	for(size_t y = 0; y < lines.size(); y++){
		for (size_t x = 0; x < lines[y].size(); x++) {
			Point p = {x, y};
			if (std::find(cycle.begin(), cycle.end(), p) == cycle.end()) {
				if(check_inside(p, cycle, lines)) {
					inside.insert(p);
					res++;
				}
			}
		}
	}

	print_cycle(cycle, inside, lines);

	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_operation(argv[1]);

	assert_no_print(eq,do_operation("test21.txt"), 8);
	assert_no_print(eq,do_operation("test22.txt"), 10);

	std::cout << "Total " << res << '\n';
	return 0;
}
