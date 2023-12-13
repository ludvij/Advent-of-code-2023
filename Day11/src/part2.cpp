// Aoc 2023 Day 11 Part 2
// author: Lud
#include <algorithm>
#include <iostream>
#include <lud_utils.hpp>
#include <numeric>
#include <vector>
#include <deque>

using Point = Lud::Vec2<s64>;

std::vector<std::vector<bool>> expand(const std::vector<std::string>& lines)
{
	std::deque<size_t> to_expand;
	std::vector<std::vector<bool>> res;
	res.resize(lines.size());

	for(size_t x = 0; x < lines[0].size(); x++) {
		bool empty = true;
		for(size_t y = 0; y < lines.size(); y++) {
			if (lines[y][x] == '#') {
				empty = false;
				break;
			}
		}
		if(empty) {
			to_expand.push_back(x);
		}
	}
	for(size_t y = 0; y < lines.size(); y++) {
		for(size_t x = 0; x < lines[y].size(); x++) {
			if (x == to_expand.front()) {
				to_expand.push_back(to_expand.front());
				to_expand.pop_front();
				res[y].push_back(true);
			} 
			else {
				res[y].push_back(false);
			}
		}
		if (lines[y].find("#") == std::string::npos) {
			std::fill(res[y].begin(), res[y].end(), true);
		}
	}
	
	return res;
}

s64 do_operation(const char* filename, const int enlarge)
{
	auto lines = Lud::Slurper::Slurp(filename);
	auto expanded = expand(lines);
	std::deque<Point> stars;
	size_t cy = 0;
	for(size_t y = 0; y < lines.size(); y++) {
		size_t cx = 0;
		for(size_t x = 0; x < lines[y].size(); x++) {
			if (lines[y][x] == '#') {
				stars.emplace_back(cx, cy);
			}
			cx += expanded[0][x] ? enlarge : 1;
		}
		cy += expanded[y][0] ? enlarge : 1;
	}
	s64 res = 0;

	while(stars.size() > 1) {
		auto o1 = stars.front();
		std::vector<s64> distance;
		distance.reserve(stars.size());
		stars.pop_front();
		for(const auto& o2: stars) {
			// manhattan distance
			distance.push_back(std::abs(o1.x - o2.x) + std::abs(o1.y - o2.y));
		}
		res += std::reduce(distance.begin(), distance.end());
	}
	return res;
}


int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " path/to/file\n";
		return 0;
	}
	const auto res = do_operation(argv[1], 1e6);

	NO_PRINT(
		Lud::assert_eq(do_operation("test.txt", 10), 1030);
		Lud::assert_eq(do_operation("test.txt", 100), 8410);
	);

	std::cout << "Total " << res << '\n';
	return 0;
}
