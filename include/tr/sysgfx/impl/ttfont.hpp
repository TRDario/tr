///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of ttfont.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../ttfont.hpp"

////////////////////////////////////////////////////////////////// TTFONT /////////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R> tr::sys::ttfont tr::sys::load_embedded_ttfont(R&& range, float size)
{
	return load_embedded_ttfont(std::span<const std::byte>{range_bytes(range)}, size);
};