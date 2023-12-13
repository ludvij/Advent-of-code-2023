// Aoc 2023 Day 10 Part 2Point
// author: Lud
#include <iostream>
#include <ostream>
#include <array>
#include <lud_utils.hpp>
#include <optional>
#include <vector>
#include <unordered_set>

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


std::array<Point, 2> interpret_s(std::vector<std::string>& lines, const Point& begin)
{
	const auto [x, y] = begin;
	std::optional<Point> p1;
	std::optional<Point> p2;
	int access = 0;
	if (y > 0 && (lines[y - 1][x] == '|' || lines[y - 1][x] == '7' || lines[y - 1][x] == 'F')) {
		p1 = {x, y-1};
		access |= 0b0001;
	}
	if (x > 0 && (lines[y][x - 1] == '-' || lines[y][x - 1] == 'L' || lines[y][x - 1] == 'F')) {
		if      (!p1) p1 = {x - 1, y};
		else if (!p2) p2 = {x - 1, y};
		access |= 0b0010;
	}
	if (x < lines[y].size() && (lines[y][x + 1] == '-' || lines[y][x + 1] == 'J' || lines[y][x + 1] == '7')) {
		if      (!p1) p1 = {x + 1, y};
		else if (!p2) p2 = {x + 1, y};
		access |= 0b0100;
	}
	if (y < lines.size() && (lines[y + 1][x] == '|' || lines[y + 1][x] == 'J' || lines[y + 1][x] == 'L')) {
		p2 = {x, y + 1};
		access |= 0b1000;
	}
	if      (access == 0b0011) lines[y] = lines[y].replace(x, 1, "J");
	else if (access == 0b0101) lines[y] = lines[y].replace(x, 1, "L");
	else if (access == 0b1001) lines[y] = lines[y].replace(x, 1, "|");
	else if (access == 0b0110) lines[y] = lines[y].replace(x, 1, "-");
	else if (access == 0b1010) lines[y] = lines[y].replace(x, 1, "7");
	else if (access == 0b1100) lines[y] = lines[y].replace(x, 1, "F");
	return {*p1, *p2};
}

// raycast
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
	return intersections % 2 == 1;
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
			if (cycle.contains(p)) {
				continue;
			}
			if(check_inside(p, cycle, lines)) {
				inside.insert(p);
				res++;
			}
		}
	}

	return res;
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
