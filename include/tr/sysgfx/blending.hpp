/// @file
/// @brief Provides blending datatypes and constants.

#pragma once

//

namespace tr
{
	/// @brief Blending function.
	enum class blend_fn
	{
		/// @brief The source and destination colors are added together.
		add = 0x8006,

		/// @brief The minimum channel values of the source and destination colors are selected.
		min,

		/// @brief The maximum channel values of the source and destination colors are selected.
		max,

		/// @brief The source and destination colors are subtracted in the form (src - dst).
		subtract = 0x800A,

		/// @brief The source and destination colors are subtracted in the form (dst - src).
		reverse_subtract
	};

	/// @brief Blending parameter multiplier.
	enum class blend_multiplier
	{
		/// @brief The parameter is multiplied by 0.
		zero,

		/// @brief The parameter is multiplied by 1.
		one,

		/// @brief The parameter is multiplied by the source color.
		src_color = 0x300,

		/// @brief The parameter is multiplied by 1 - the source color.
		one_minus_src_color,

		/// @brief The parameter is multiplied by the source alpha.
		src_alpha,

		/// @brief The parameter is multiplied by 1 - the source alpha.
		one_minus_src_alpha,

		/// @brief The parameter is multiplied by the destination alpha.
		dst_alpha,

		/// @brief The parameter is multiplied by 1 - the destination alpha.
		one_minus_dst_alpha,

		/// @brief The parameter is multiplied by the destination color.
		dst_color,

		/// @brief The parameter is multiplied by 1 - the destination color.
		one_minus_dst_color
	};

	/// @brief Blending mode information.
	struct blend_mode
	{
		/// @brief Multiplier used for the source color.
		blend_multiplier rgb_src;

		/// @brief Operation applied with the source and destination colors.
		blend_fn rgb_fn;

		/// @brief Multiplier used for the destination color.
		blend_multiplier rgb_dst;

		/// @brief Multiplier used for the source alpha.
		blend_multiplier alpha_src;

		/// @brief Operation applied with the source and destination alpha.
		blend_fn alpha_fn;

		/// @brief Multiplier used for the destination alpha.
		blend_multiplier alpha_dst;

		//

		/// @brief Checks two blending modes for equality.
		/// @param lhs, rhs Blending modes to compare.
		/// @return `true` if the blending modes are equal, `false` otherwise.
		[[nodiscard]] friend bool operator==(const blend_mode& lhs, const blend_mode& rhs) noexcept = default;
	};

	//

	/// @brief Alpha blending mode.
	inline constexpr blend_mode alpha_blending{blend_multiplier::src_alpha, blend_fn::add, blend_multiplier::one_minus_src_alpha,
											   blend_multiplier::one,       blend_fn::add, blend_multiplier::one_minus_src_alpha};

	/// @brief Premultiplied alpha blending mode.
	inline constexpr blend_mode premultiplied_alpha_blending{blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha,
															 blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha};
} // namespace tr