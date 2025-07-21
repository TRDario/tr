#pragma once
#include "angle.hpp"

namespace tr {
	// Generates a random seed.
	std::uint64_t gen_random_seed() noexcept;

	// Xorshiftr128+ random number generator.
	class xorshiftr_128p {
	  public:
		// Initializes RNG with a value gotten from gen_random_seed().
		xorshiftr_128p() noexcept;
		// Initializes RNG with a seed.
		constexpr xorshiftr_128p(std::uint64_t seed) noexcept;

		// Advances the RNG and returns a value.
		constexpr std::uint64_t advance() noexcept;

	  private:
		// The internal state of the generator.
		std::uint64_t _state[2];
	};

	// Generates a random boolean value.
	constexpr bool randbool(xorshiftr_128p& rng) noexcept;
	// Generates a random sign (-1 or 1).
	constexpr int randsign(xorshiftr_128p& rng) noexcept;
	// Generates a random integral value.
	template <std::integral T> constexpr T rand(xorshiftr_128p& rng) noexcept;
	// Generates a random integral value in the range [0, max).
	template <std::integral T> constexpr T rand(xorshiftr_128p& rng, T max) noexcept;
	// Generates a random integral value in the range [min, max).
	template <std::integral T> constexpr T rand(xorshiftr_128p& rng, T min, T max) noexcept;
	// Generates a random floating point value in the range [0, 1).
	template <std::floating_point T> constexpr T rand(xorshiftr_128p& rng) noexcept;
	// Generates a random floating point value in the range [0, max).
	template <std::floating_point T> constexpr T rand(xorshiftr_128p& rng, T max) noexcept;
	// Generates a random floating point value in the range [min, max).
	template <std::floating_point T> constexpr T rand(xorshiftr_128p& rng, T min, T max) noexcept;
	// Generates a random angle value in the range [0tr, 1tr).
	template <specialization_of<tr::angle> T> constexpr T rand(xorshiftr_128p& rng) noexcept;
	// Generates a random angle value in the range [0, max).
	template <specialization_of<tr::angle> T> constexpr T rand(xorshiftr_128p& rng, T max) noexcept;
	// Generates a random angle value in the range [min, max).
	template <specialization_of<tr::angle> T> constexpr T rand(xorshiftr_128p& rng, T min, T max) noexcept;
	// Generates a random vector with a certain magnitude.
	template <std::floating_point T> glm::tvec2<T> randvec(xorshiftr_128p& rng, T magnitude) noexcept;
} // namespace tr

#include "rng_impl.hpp"