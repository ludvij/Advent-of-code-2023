#ifndef LUD_PARSER_HEADER
#define LUD_PARSER_HEADER
#include <charconv>
#include <string_view>
#include "lud_core.hpp"

namespace Lud {

template<NumberType N> N parse_num(std::string_view sv);



template<NumberType N> N parse_num(std::string_view sv)
{
	N val = 0;
	std::from_chars(sv.data(), sv.data() + sv.size(), val);
	return val;
}

}
#endif