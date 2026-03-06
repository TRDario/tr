///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of encryption.hpp.                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../encryption.hpp"
#include "../ranges.hpp"

//////////////////////////////////////////////////////////////// ENCRYPTION ///////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range Range> void tr::encrypt_to(std::vector<std::byte>& out, Range&& range)
{
	encrypt_to(out, std::span<const std::byte>{range_bytes(range)});
}

template <std::ranges::contiguous_range Range> std::vector<std::byte> tr::encrypt(Range&& range)
{
	return encrypt(std::span<const std::byte>{range_bytes(range)});
}