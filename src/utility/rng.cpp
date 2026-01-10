///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements rng.hpp.                                                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/rng.hpp"
#include <ctime>
#include <random>

///////////////////////////////////////////////////////////// GEN RANDOM SEED /////////////////////////////////////////////////////////////

tr::u64 tr::gen_random_seed()
{
	std::random_device rng;
	return rng() ^ std::time(nullptr);
}

////////////////////////////////////////////////////////////// XORSHIFTR128+ //////////////////////////////////////////////////////////////

tr::xorshiftr_128p::xorshiftr_128p(u64 seed)
	: m_state{seed++, seed}
{
	advance();
	advance();
}

tr::u64 tr::xorshiftr_128p::advance()
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

int tr::xorshiftr_128p::generate_sign()
{
	return generate(2) * 2 - 1;
}

tr::angle tr::xorshiftr_128p::generate(angle max)
{
	return rads(generate(max.rads()));
}

tr::angle tr::xorshiftr_128p::generate(angle min, angle max)
{
	TR_ASSERT(min < max, "RNG range minimum must be less than the maximum (Currenly: {:r} !< {:r})", min, max);

	return rads(generate(min.rads(), max.rads()));
}