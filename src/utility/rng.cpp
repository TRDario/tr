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

//

bool tr::generate_bool(rng& rng) noexcept
{
	return rng.advance() >> 63;
}

int tr::generate_sign(rng& rng) noexcept
{
	return static_cast<int>(rng.advance() >> 63) * 2 - 1;
}