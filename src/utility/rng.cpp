#include "../../include/tr/utility/rng.hpp"
#include <ctime>
#include <random>

std::uint64_t tr::gen_random_seed() noexcept
{
	std::random_device rng;
	return rng() ^ std::time(nullptr);
}

tr::xorshiftr_128p::xorshiftr_128p() noexcept
	: xorshiftr_128p{gen_random_seed()}
{
}