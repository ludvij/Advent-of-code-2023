#ifndef LUD_VEC2_HEADER
#define LUD_VEC2_HEADER
#include "lud_core.hpp"
#include <compare>
#include <functional>
#include <ostream>

namespace Lud {
template<NumberType N>
struct Vec2 {
	N x;
	N y;

	std::weak_ordering operator<=>(const Vec2& o) const = default;
	bool operator==(const Vec2& o) const = default;
};

template<NumberType N> inline std::ostream& operator<<(std::ostream& os, const Vec2<N>& v) 
{
	os << '[' << v.x << ", " << v.y << ']';
	return os;
}

}

template <NumberType N> struct std::hash<Lud::Vec2<N>> {
	size_t operator()(const Lud::Vec2<N>& s) const noexcept {
		const size_t x = std::hash<size_t>{}(s.x);
		const size_t y = std::hash<size_t>{}(s.y);

		return (x * 73856093) ^ y;
	}
};
#endif