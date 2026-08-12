/// @file
/// @brief Implements the templated parts of bitmap_view.hpp.

#pragma once
#include "../../utility/ranges.hpp"
#include "../bitmap_view.hpp"

//

template <std::ranges::contiguous_range Range>
tr::bitmap_view::bitmap_view(Range&& range, glm::ivec2 size, pixel_format format)
	: bitmap_view{std::span<const std::byte>{range_bytes(range)}, size, format}
{
}