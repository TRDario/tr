///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an implementation of the Xorshiftr128+ random number generator.                                                              //
//                                                                                                                                       //
// tr::gen_random_seed() generates a random unsigned 64-bit integer. While it can be used as a random source in its own right,           //
// it's intended purpose is to seed the Xorshiftr128+ RNG (done automatically in the default constructor):                               //
//     - gen_random_seed() -> [0, 2^64-1]ULL                                                                                             //
//                                                                                                                                       //
// A raw random unsigned 64-bit integer value can be generated using the Xorshiftr128+ generator using advance(), which can be used as a //
// base for further custom generation functions:                                                                                         //
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
// taking a rect and returning a point in that rect:                                                                                     //
//     - rng.generate<glm::vec2>(1.0f) -> a random unit vector                                                                           //
//     - rng.generate<glm::vec2>({{2, 2}, {1, 3}}) -> {[2.0f, 3.0f), [2.0f, 5.0f)}                                                       //
//                                                                                                                                       //
// A random sign (+/-1) can be generated with generate_sign:                                                                             //
//     - rng.generate(2, 4) * rng.generate_sign() -> one of 2, 3, -2, -3                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "geometry.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Generates a random seed.
	u64 generate_random_seed();

	// Xorshiftr128+ random number generator.
	class rng {
	  public:
		// Initializes RNG.
		rng(u64 seed = generate_random_seed());

		// Advances the RNG and returns a value.
		u64 advance();

		// Generates a random integral value.
		template <std::integral Integer> Integer generate();
		// Generates a random integral value in the range [0, max).
		template <std::integral Integer> Integer generate(Integer max);
		// Generates a random integral value in the range [min, max).
		template <std::integral Integer> Integer generate(Integer min, Integer max);
		// Generates a random floating point value in the range [0, 1).
		template <std::floating_point FloatingPoint> FloatingPoint generate();
		// Generates a random floating point value in the range [0, max).
		template <std::floating_point FloatingPoint> FloatingPoint generate(FloatingPoint max);
		// Generates a random floating point value in the range [min, max).
		template <std::floating_point FloatingPoint> FloatingPoint generate(FloatingPoint min, FloatingPoint max);
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

#include "impl/rng.hpp" // IWYU pragma: export