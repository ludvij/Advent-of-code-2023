#ifndef LUD_PARSER_HEADER
#define LUD_PARSER_HEADER
#include <charconv>
#include <cstdint>

namespace Lud {

template<typename NumberType> NumberType parse_num(std::string_view sv);



template<typename NumberType> NumberType parse_num(std::string_view sv)
{
	NumberType val = 0;
	std::from_chars(sv.data(), sv.data() + sv.size(), val);
	return val;
}

}
#endif