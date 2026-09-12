/// @file
/// @brief Implements rng.hpp.

#include <ctime>
#include <random>
#include <tr/utility/rng.hpp>

//

tr::u64 tr::generate_random_seed()
{
	std::random_device rng;
	return rng() ^ std::time(nullptr);
}

//

tr::rng::rng(u64 seed) noexcept
	: m_state{seed++, seed}
{
	advance();
	advance();
}

tr::u64 tr::rng::advance() noexcept
{
	u64 x{m_state[0]};
	u64 y{m_state[1]};
	m_state[0] = y;
	x ^= x << 23;
	x ^= x >> 17;
	x ^= y;
	m_state[1] = x + y;
	return x;
}

int tr::rng::generate_sign() noexcept
{
	return generate<bool>() * 2 - 1;
}

tr::angle tr::rng::generate(angle max) noexcept
{
	return rads(generate(max.rads()));
}

tr::angle tr::rng::generate(angle min, angle max) noexcept
{
	TR_ASSERT(min < max, "RNG range minimum must be less than the maximum (Currenly: {:r} !< {:r})", min, max);

	return rads(generate(min.rads(), max.rads()));
}