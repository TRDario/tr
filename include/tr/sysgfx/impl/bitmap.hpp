///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of bitmap.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../../utility/ranges.hpp"
#include "../bitmap.hpp"

/////////////////////////////////////////////////////////////// BITMAP VIEW ///////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range T>
tr::bitmap_view::bitmap_view(T&& range, glm::ivec2 size, pixel_format format)
	: bitmap_view{std::span<const std::byte>{range_bytes(range)}, size, format}
{
}

////////////////////////////////////////////////////////////////// BITMAP /////////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R> tr::bitmap tr::load_embedded_bitmap(R&& range)
{
	return load_embedded_bitmap(std::span<const std::byte>{range_bytes(range)});
}
