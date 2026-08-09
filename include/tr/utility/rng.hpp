/// @file
/// @brief Provides an implementation of the Xorshiftr128+ random number generator.

#pragma once
#include "angle.hpp"
#include "rectangle.hpp"

//

namespace tr
{
	/// Generates a random seed.
	/// @return Random 64-bit seed.
	u64 generate_random_seed();

	/// Xorshiftr128+ random number generator.
	class rng
	{
	  public:
		/// @name Constructors
		/// @{

		/// Initializes RNG.
		/// @param seed Seed to initialize the generator with.
		rng(u64 seed = generate_random_seed());

		/// @}
		/// @name Raw
		/// @{

		/// Advances the RNG and returns a value.
		/// @return New randomly-generated value.
		u64 advance();

		/// @}
		/// @name Generation
		/// @{

		/// Generates a random integral value.
		/// @tparam Integer Integer type.
		/// @return Random integral value.
		template <std::integral Integer>
		Integer generate();

		/// Generates a random integral value in the range [0, `max`).
		/// @tparam Integer Integer type.
		/// @param max Upper bound of the random range.
		/// @pre `max` must be greater than 0.
		/// @return Random integral value constrained to [0, `max`).
		template <std::integral Integer>
		Integer generate(Integer max);

		/// Generates a random integral value in the range [`min`, `max`).
		/// @tparam Integer Integer type.
		/// @param min Lower bound of the random range.
		/// @param max Upper bound of the random range.
		/// @pre `min` must be less than `max`.
		/// @return Random integral value constrained to [`min`, `max`).
		template <std::integral Integer>
		Integer generate(Integer min, Integer max);

		/// Generates a random floating point value in the range [0, 1).
		/// @tparam FloatingPoint Floating-point type.
		/// @return Random floating-point value constrained to [0, 1).
		template <std::floating_point FloatingPoint>
		FloatingPoint generate();

		/// Generates a random floating point value in the range [0, `max`).
		/// @tparam FloatingPoint Floating-point type.
		/// @param max Upper bound of the random range.
		/// @pre `max` must be greater than 0.
		/// @return Random floating-point value constrained to [0, `max`).
		template <std::floating_point FloatingPoint>
		FloatingPoint generate(FloatingPoint max);

		/// Generates a random floating point value in the range [`min`, `max`).
		/// @tparam FloatingPoint Floating-point type.
		/// @param min Lower bound of the random range.
		/// @param max Upper bound of the random range.
		/// @pre `min` must be less than `max`.
		/// @return Random floating-point value constrained to [`min`, `max`).
		template <std::floating_point FloatingPoint>
		FloatingPoint generate(FloatingPoint min, FloatingPoint max);

		/// Generates a random angle value in the range [0tr, 1tr).
		/// @return Random angle in the range [0tr, 1tr).
		template <std::same_as<angle>>
		angle generate();

		/// Generates a random angle value in the range [0, `max`).
		/// @param max Upper bound of the random range.
		/// @pre `max` must be greater than 0.
		/// @return Random angle in the range [0, `max`).
		angle generate(angle max);

		/// Generates a random angle value in the range [`min`, `max`).
		/// @param min Lower bound of the random range.
		/// @param max Upper bound of the random range.
		/// @pre `min` must be less than `max`.
		/// @return Random angle in the range [`min`, `max`).
		angle generate(angle min, angle max);

		/// Generates a random vector with a specific magnitude.
		/// @param magnitude Magnitude of the vector.
		/// @return Vector with a set magnitude and random direction.
		template <std::same_as<glm::vec2>>
		glm::vec2 generate(float magnitude);

		/// Generates a random point within a rectangular region.
		/// @param region Region to constrain the vector to.
		/// @return Vector to a random point within `region`.
		template <std::same_as<glm::vec2>>
		glm::vec2 generate(const tr::rectangle<float>& region);

		/// Generates a random sign (-1 or 1).
		/// @return `-1` or `1`.
		int generate_sign();

		/// @}

	  private:
		/// Internal state of the generator.
		u64 m_state[2];
	};
} // namespace tr

#include "impl/rng.hpp" // IWYU pragma: export