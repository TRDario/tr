///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements print.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../print.hpp"

////////////////////////////////////////////////////////////////// PRINT //////////////////////////////////////////////////////////////////

template <typename... Args> void tr::print_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args)
{
	std::format_to(std::ostreambuf_iterator<char>{out}, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::print(std::format_string<Args...> fmt, Args&&... args)
{
	print_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::print_error(std::format_string<Args...> fmt, Args&&... args)
{
	print_to(std::cerr, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::println_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args)
{
	print_to(out, fmt, std::forward<Args>(args)...);
	print_to(out, "\n");
}

template <typename... Args> void tr::println(std::format_string<Args...> fmt, Args&&... args)
{
	println_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::println_error(std::format_string<Args...> fmt, Args&&... args)
{
	println_to(std::cerr, fmt, std::forward<Args>(args)...);
}