///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides formatted printing functions.                                                                                                //
//                                                                                                                                       //
// tr::print_to prints formatted text to an output stream as if by std::format_to.                                                       //
// tr::print is equivalent to tr::print_to(std::cout).                                                                                   //
// tr::print_error is equivalent to tr::print_to(std::cerr).                                                                             //
//     - tr::print("PI = {:.2f}!", std::numbers::pi) -> "PI = 3.14!"                                                                     //
//                                                                                                                                       //
// tr::println_to prints formatted text to an output stream as if by std::format_to and inserts a newline at the end.                    //
// tr::println is equivalent to tr::println_to(std::cout).                                                                               //
// tr::println_error is equivalent to tr::println_to(std::cerr).                                                                         //
//     - tr::println("PI = {:.2f}!", std::numbers::pi) -> "PI = 3.14!\n"                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Prints to a stream.
	template <typename... Args> void print_to(std::ostream& out, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
	// Prints to the standard output.
	template <typename... Args> void print(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
	// Prints to the standard error output.
	template <typename... Args> void print_error(TR_FORMAT_STRING<Args...> fmt, Args&&... args);

	// Prints a line to a stream.
	template <typename... Args> void println_to(std::ostream& out, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
	// Prints a line to the standard output.
	template <typename... Args> void println(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
	// Prints a line to the standard error output.
	template <typename... Args> void println_error(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
} // namespace tr

#include "impl/print.hpp" // IWYU pragma: export