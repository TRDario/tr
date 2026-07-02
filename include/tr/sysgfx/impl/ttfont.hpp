///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of ttfont.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../ttfont.hpp"

/////////////////////////////////////////////////////////////// TTFONT ERROR //////////////////////////////////////////////////////////////

template <typename... Args>
tr::ttfont_error::ttfont_error(TR_FORMAT_STRING<Args...> description_fmt, Args&&... args)
	: ttfont_error{TR_FMT::format(description_fmt, std::forward<Args>(args)...)}
{
}

////////////////////////////////////////////////////////////////// TTFONT /////////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R> tr::ttfont tr::load_embedded_ttfont(R&& range, float size)
{
	return load_embedded_ttfont(std::span<const std::byte>{range_bytes(range)}, size);
};