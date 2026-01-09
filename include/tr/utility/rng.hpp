///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an implementation of the Xorshiftr128+ random number generator.                                                              //
//                                                                                                                                       //
// tr::gen_random_seed() generates a random unsigned 64-bit integer. While it can be used as a random source in its own right,           //
// it's intended purpose is to seed the Xorshiftr128+ RNG (done automatically in the default constructor).                               //
//     - gen_random_seed() -> [0, 2^64-1]ULL                                                                                             //
//                                                                                                                                       //
// A raw random unsigned 64-bit integer value can be generated using the Xorshiftr128+ generator using advance(), which can be used as a //
// base for further custom generation functions.                                                                                         //
//     - rng.advance() -> [0, 2^64-1]ULL                                                                                                 //
//                                                                                                                                       //
// Random numeric or angle values can be generated using the generate method with one of three overloads:                                //
// generate() generates a value in the default range ([0, MAX] for integers, [0, 1) for floating-point numbers, [0tr, 1tr) for angles).  //
// generate(max) generates a value in the range [0, max).                                                                                //
// generate(min, max) generates a value in the range [min, max).                                                                         //
//     - rng.generate<i8>() -> u8([0, 255])                                                                                              //
//     - rng.generate<float>() -> [0.0f, 1.0f)                                                                                           //
//     - rng.generate(10) -> [0, 10)                                                                                                     //
//     - rng.generate(-1.5f, 1.5f) -> [-1.5f, 1.5f)                                                                                      //
//                                                                                                                                       //
// There are special overloads of generate for glm::vec2: one taking a magnitude and returning a vector of that magnitude, and another   //
// taking a rect and returning a point in that rect.                                                                                     //
//     - rng.generate<glm::vec2>(1.0f) -> a random unit vector                                                                           //
//     - rng.generate<glm::vec2>({{2, 2}, {1, 3}}) -> {[2.0f, 3.0f), [2.0f, 5.0f)}                                                       //
//                                                                                                                                       //
// A random sign (+/-1) can be generated with generate_sign.                                                                             //
//     - rng.generate(2, 4) * rng.generate_sign() -> one of 2, 3, -2, -3                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "angle.hpp"
#include "macro.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Generates a random seed.
	u64 gen_random_seed();

	// Xorshiftr128+ random number generator.
	class xorshiftr_128p {
	  public:
		// Initializes RNG.
		xorshiftr_128p(u64 seed = gen_random_seed());

		// Advances the RNG and returns a value.
		u64 advance();

		// Generates a random integral value.
		template <std::integral T> T generate();
		// Generates a random integral value in the range [0, max).
		template <std::integral T> T generate(T max);
		// Generates a random integral value in the range [min, max).
		template <std::integral T> T generate(T min, T max);
		// Generates a random floating point value in the range [0, 1).
		template <std::floating_point T> T generate();
		// Generates a random floating point value in the range [0, max).
		template <std::floating_point T> T generate(T max);
		// Generates a random floating point value in the range [min, max).
		template <std::floating_point T> T generate(T min, T max);
		// Generates a random angle value in the range [0tr, 1tr).
		template <std::same_as<angle>> angle generate();
		// Generates a random angle value in the range [0, max).
		angle generate(angle max);
		// Generates a random angle value in the range [min, max).
		angle generate(angle min, angle max);
		// Generates a random vector with a certain magnitude.
		template <std::same_as<glm::vec2>> glm::vec2 generate(float magnitude);
		// Generates a random point within a rectangular region.
		template <std::same_as<glm::vec2>> glm::vec2 generate(const tr::frect2& rect);

		// Generates a random sign (-1 or 1).
		int generate_sign();

	  private:
		// The internal state of the generator.
		u64 m_state[2];
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::integral T> T tr::xorshiftr_128p::generate()
{
	// Upper bits of the value generated by advance() have better randomness.
	if constexpr (std::same_as<T, bool>) {
		return advance() >> 63;
	}
	else {
		return T(advance() >> ((sizeof(advance()) - sizeof(T)) * 4));
	}
}

template <std::integral T> T tr::xorshiftr_128p::generate(T max)
{
	TR_ASSERT(max > 0, "RNG range maximum must be greater than 0 (Currently: {}).", max);

	return T(generate<std::make_unsigned_t<T>>() % max);
}

template <std::integral T> T tr::xorshiftr_128p::generate(T min, T max)
{
	TR_ASSERT(min < max, "RNG range minimum must be less than the maximum (Currenly: {} !< {})", min, max);

	return T(generate<std::make_unsigned_t<T>>() % (max - min) + min);
}

template <std::floating_point T> T tr::xorshiftr_128p::generate()
{
	return T(advance()) / T(UINT64_MAX);
}

template <std::floating_point T> T tr::xorshiftr_128p::generate(T max)
{
	return generate<T>() * max;
}

template <std::floating_point T> T tr::xorshiftr_128p::generate(T min, T max)
{
	TR_ASSERT(min < max, "RNG range minimum must be less than the maximum (Currenly: {} !< {})", min, max);

	return generate<T>() * (max - min) + min;
}

template <std::same_as<tr::angle>> tr::angle tr::xorshiftr_128p::generate()
{
	return turns(generate<float>());
}

template <std::same_as<glm::vec2>> glm::vec2 tr::xorshiftr_128p::generate(float magnitude)
{
	// Prefer this over generating an angle because trigonometric functions don't produce uniform results across platforms.
	const float sin2{generate<float>()};
	const float sin{std::sqrt(sin2) * generate_sign()};
	const float cos{std::sqrt(1 - sin2) * generate_sign()};
	return {magnitude * cos, magnitude * sin};
}

template <std::same_as<glm::vec2>> glm::vec2 tr::xorshiftr_128p::generate(const tr::frect2& rect)
{
	return {generate(rect.tl.x, rect.tl.x + rect.size.x), generate(rect.tl.y, rect.tl.y + rect.size.y)};
}