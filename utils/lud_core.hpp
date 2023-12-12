#ifndef LUD_CORE_HEADER
#define LUD_CORE_HEADER

#include <type_traits>
#include <cstdint>



template<typename T>
concept NumberType = requires(T param)
{
    requires std::is_integral_v<T> || std::is_floating_point_v<T>;
    requires !std::is_same_v<bool, T>;
    requires std::is_arithmetic_v<decltype(param +1)>;
    requires !std::is_pointer_v<T>;
};


// some typedefs
using u8 = uint8_t;
using s8 =  int8_t;
using u16 = uint16_t;
using s16 =  int16_t;
using u32 = uint32_t;
using s32 =  int32_t;
using u64 = uint64_t;
using s64 =  int64_t;


#endif