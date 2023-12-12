#ifndef LUD_ASSERT_HEADER
#define LUD_ASSERT_HEADER

#include <cstdlib>
#include <iostream>
#include <source_location>
#include <sstream>

namespace Lud {

// hacky workaround when there are a lot of prints
#define NO_PRINT(code) \
{\
	std::streambuf *old_cout = std::cout.rdbuf(); \
	std::stringstream ss; \
	std::cout.rdbuf(ss.rdbuf()); \
	code \
	std::cout.rdbuf(old_cout); \
}

#define assert_no_print(assert_type, fn, val) NO_PRINT(Lud::assert_##assert_type(fn, val))

template<class T1, class T2> void assert_eq(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());
template<class T1, class T2> void assert_ne(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());
template<class T1, class T2> void assert_greater(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());
template<class T1, class T2> void assert_lower(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());
template<class T1, class T2> void assert_geq(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());
template<class T1, class T2> void assert_leq(T1 n1, T2 n2, const std::source_location loc = std::source_location::current());

template<class T1, class T2> void log_assertion_fail(const T1 n1, const T2 n2, const char* msg, const std::source_location loc);





template<class T1, class T2> void assert_eq(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 == n2) return;

	log_assertion_fail(n1, n2, " != ", loc);
	std::exit(EXIT_FAILURE);
}

template<class T1, class T2> void assert_ne(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 != n2) return;

	log_assertion_fail(n1, n2, " == ", loc);
	std::exit(EXIT_FAILURE);
}

template<class T1, class T2> void assert_greater(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 > n2) return;

	log_assertion_fail(n1, n2, " is not greater than ", loc);
	std::exit(EXIT_FAILURE);
}

template<class T1, class T2> void assert_lower(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 < n2) return;

	log_assertion_fail(n1, n2, " is not lower than ", loc);
	std::exit(EXIT_FAILURE);
}

template<class T1, class T2> void assert_geq(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 >= n2) return;

	log_assertion_fail(n1, n2, " is not greater or equal to ", loc);
	std::exit(EXIT_FAILURE);
}

template<class T1, class T2> void assert_leq(T1 n1, T2 n2, const std::source_location loc)
{
	if (n1 <= n2) return;

	log_assertion_fail(n1, n2, " is not lower or equal to ", loc);
	std::exit(EXIT_FAILURE);
}


template<class T1, class T2> void log_assertion_fail(const T1 n1, const T2 n2, const char* msg, const std::source_location loc)
{
	std::clog << "[ASSERT FAIL] " << loc.file_name() << '(' << loc.line() << ':' << loc.column() << ")\n"
	          << "              " << n1 << msg << n2 << '\n';
}

}
#endif//LUD_ASSERT_HEADER