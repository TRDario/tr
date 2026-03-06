///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements print.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../print.hpp"

////////////////////////////////////////////////////////////////// PRINT //////////////////////////////////////////////////////////////////

template <typename... Args> void tr::print_to(std::ostream& out, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	std::format_to(std::ostreambuf_iterator<char>{out}, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::print(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	print_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::print_error(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	print_to(std::cerr, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::println_to(std::ostream& out, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	print_to(out, fmt, std::forward<Args>(args)...);
	print_to(out, "\n");
}

template <typename... Args> void tr::println(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	println_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <typename... Args> void tr::println_error(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	println_to(std::cerr, fmt, std::forward<Args>(args)...);
}