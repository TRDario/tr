/// @file
/// @brief Implements the templated parts of ttfont.hpp.

#pragma once
#include "../ttfont.hpp"

//

template <typename... Args>
tr::ttfont_error::ttfont_error(std::format_string<Args...> description_fmt, Args&&... args)
	: ttfont_error{std::format(description_fmt, std::forward<Args>(args)...)}
{
}

//

template <std::ranges::contiguous_range Range>
tr::ttfont tr::load_embedded_ttfont(Range&& range, float size)
{
	return load_embedded_ttfont(std::span<const std::byte>{range_bytes(range)}, size);
};