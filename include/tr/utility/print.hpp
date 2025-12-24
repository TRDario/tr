#pragma once
#include "common.hpp"

namespace tr {
	// Prints to a stream.
	template <class... Args> void print_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args);
	// Prints to the standard output.
	template <class... Args> void print(std::format_string<Args...> fmt, Args&&... args);
	// Prints to the standard error output.
	template <class... Args> void print_error(std::format_string<Args...> fmt, Args&&... args);

	// Prints a line to a stream.
	template <class... Args> void println_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args);
	// Prints a line to the standard output.
	template <class... Args> void println(std::format_string<Args...> fmt, Args&&... args);
	// Prints a line to the standard error output.
	template <class... Args> void println_error(std::format_string<Args...> fmt, Args&&... args);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class... Args> void tr::print_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args)
{
	std::format_to(std::ostreambuf_iterator<char>{out}, fmt, std::forward<Args>(args)...);
}

template <class... Args> void tr::print(std::format_string<Args...> fmt, Args&&... args)
{
	print_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <class... Args> void tr::print_error(std::format_string<Args...> fmt, Args&&... args)
{
	print_to(std::cerr, fmt, std::forward<Args>(args)...);
}

template <class... Args> void tr::println_to(std::ostream& out, std::format_string<Args...> fmt, Args&&... args)
{
	print_to(out, fmt, std::forward<Args>(args)...);
	print_to(out, "\n");
}

template <class... Args> void tr::println(std::format_string<Args...> fmt, Args&&... args)
{
	println_to(std::cout, fmt, std::forward<Args>(args)...);
}

template <class... Args> void tr::println_error(std::format_string<Args...> fmt, Args&&... args)
{
	println_to(std::cerr, fmt, std::forward<Args>(args)...);
}