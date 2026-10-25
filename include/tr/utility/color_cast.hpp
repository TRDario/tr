/// @file
/// @brief Provides an interface for color casting.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>

namespace tr
{
	template <arithmetic Element>
	struct rgba;
}

//

namespace tr
{
	/// Interface for custom casters for use in `tr::color_cast`.
	/// @tparam From Source color type.
	/// @tparam To Destination color type.
	template <typename From, typename To>
	struct color_caster
	{
		static_assert("color_caster is not specialized for this combination of types!");

		/// Casts a color from `From` to an equivalent value of type `To`.
		/// @param from Color to cast.
		/// @return Equivalent color value of type `To`.
		static constexpr To operator()(const From& from) noexcept = delete;
	};

	/// @name Color casting concepts
	/// @{

	/// Type that is directly castable to `To` using `tr::color_cast()`.
	/// @tparam To Color type to cast to.
	template <typename Target, typename To>
	concept directly_color_castable_to = requires(const Target& from) {
		{ color_caster<Target, To>{}(from) } -> std::same_as<To>;
	};

	/// Type that is indirectly castable to `To` using `tr::color_cast()`.
	/// @details For `Target` to be indirectly castable to `To`, it must be castable to `tr::rgba<double>`, and `tr::rgba<double>` must be
	/// castable to `To`.
	/// @tparam To Color type to cast to.
	template <typename Target, typename To>
	concept indirectly_color_castable_to = requires(const Target& from, const rgba<double>& pivot) {
		{ color_caster<Target, rgba<double>>{}(from) } -> std::same_as<rgba<double>>;
		{ color_caster<rgba<double>, To>{}(pivot) } -> std::same_as<To>;
	};

	/// Type that is castable to `To` using `tr::color_cast()` directly or indirectly.
	template <typename Target, typename To>
	concept color_castable_to = std::same_as<Target, To> || directly_color_castable_to<Target, To> ||
								indirectly_color_castable_to<Target, To>;

	/// @}
	/// @name Casts
	/// @{

	/// Casts a color to an equivalent value of type `To`.
	/// @tparam To Destination color type.
	/// @tparam From Source color type.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `To`.
	template <typename To, color_castable_to<To> From>
	[[nodiscard]] constexpr To color_cast(const From& from) noexcept
	{
		if constexpr (std::same_as<From, To>) {
			return from;
		}
		else if constexpr (directly_color_castable_to<From, To>) {
			return color_caster<From, To>{}(from);
		}
		else {
			return color_caster<rgba<double>, To>{}(color_caster<From, rgba<double>>{}(from));
		}
	}

	/// @}
} // namespace tr