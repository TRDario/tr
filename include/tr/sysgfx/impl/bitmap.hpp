/// @file
/// @brief Implements the templated parts of bitmap.hpp.

#pragma once
#include "../../utility/ranges.hpp"
#include "../bitmap.hpp"

//

template <std::ranges::contiguous_range Range>
tr::bitmap tr::load_embedded_bitmap(Range&& range)
{
	return load_embedded_bitmap(std::span<const std::byte>{range_bytes(range)});
}
