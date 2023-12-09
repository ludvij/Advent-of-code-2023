#include <iostream>
#include <string_view>
#include <algorithm>
#include <unordered_map>

#include "ctre.hpp"
#include "lud_utils.hpp"

class Hand {
public:
	Hand(const std::string_view& hand, const int bid)
		: value(bid)
	{
		std::unordered_map<char, int> map;
		for(size_t i = 0; i < 5; i++) {
			const char c = hand[i];
			switch(c) {
				case '2': cards[i] =  0; break;
				case '3': cards[i] =  1; break;
				case '4': cards[i] =  2; break;
				case '5': cards[i] =  2; break;
				case '6': cards[i] =  3; break;
				case '7': cards[i] =  4; break;
				case '8': cards[i] =  6; break;
				case '9': cards[i] =  7; break;
				case 'T': cards[i] =  8; break;
				case 'J': cards[i] =  9; break;
				case 'Q': cards[i] = 10; break;
				case 'K': cards[i] = 11; break;
				case 'A': cards[i] = 12; break;
			}
			map[c]++;
		}
		int pairs = 0;
		int three = 0;
		for(const auto& [k, c] : map) {
			if      (c <= 1) continue;
			else if (c == 2) pairs++;
			else if (c == 3) three++;
			else if (c == 4) type = 5;
			else if (c == 5) type = 6;
		}
		if (type == 0) {
			if (pairs == 1 && three == 1) type = 4;
			else if (three == 1)          type = 3;
			else if (pairs == 2)          type = 2;
			else if (pairs == 1)          type = 1;
			else                          type = 0;
		}
	}
	
	int value;
	int type = 0;
	std::array<int, 5> cards;
};

uint64_t do_cards(const char* filename)
{
	uint64_t res = 0;
	Lud::Slurper file(filename);
	std::vector<Hand> hands;
	const auto hand_sort = [](const Hand& h1, const Hand& h2) -> bool
	{
		if (h1.type < h2.type) {
			return true;
		} 
		else if (h1.type > h2.type) {
			return false;
		}
		else {
			for(int i = 0; i < 5; i++) {
				if ( h1.cards[i] == h2.cards[i]) {
					continue;
				} 
				if ( h1.cards[i] < h2.cards[i]) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		return false;
	};
	for(const auto& line : file.ReadLines()) {
		const auto [_, hand, bid] = ctre::match<"(.{5}) (\\d+)">(line);
		hands.emplace_back(hand, Lud::parse_num<int>(bid));
	}
	std::sort(hands.begin(), hands.end(), hand_sort);

	for(size_t i = 0; i < hands.size(); i++) {
		res += (i + 1) * hands[i].value;
	}

	return res;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Usage " << argv[0] << " path/to/file\n";
		return 0;
	}

	const auto res = do_cards(argv[1]);
	std::cout << "Total " << res << '\n';
	return 0;
}