///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of window.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../window.hpp"

/////////////////////////////////////////////////////////////// WINDOW ERROR //////////////////////////////////////////////////////////////

template <typename... Args>
tr::window_error::window_error(std::format_string<Args...> description_fmt, Args&&... args)
	: window_error{std::format(description_fmt, std::forward<Args>(args)...)}
{
}