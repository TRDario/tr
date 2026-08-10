/// @file
/// @brief Provides blending datatypes and constants.

#pragma once

//

namespace tr
{
	/// Blending functions.
	enum class blend_fn
	{
		/// The source and destination colors are added together.
		add = 0x8006,
		/// The minimum channel values of the source and destination colors are selected.
		min,
		/// The maximum channel values of the source and destination colors are selected.
		max,
		/// The source and destination colors are subtracted in the form (src - dst).
		subtract = 0x800A,
		/// The source and destination colors are subtracted in the form (dst - src).
		reverse_subtract
	};

	/// Blending parameter multipliers.
	enum class blend_multiplier
	{
		/// The parameter is multiplied by 0.
		zero,
		/// The parameter is multiplied by 1.
		one,
		/// The parameter is multiplied by the source color.
		src_color = 0x300,
		/// The parameter is multiplied by 1 - the source color.
		one_minus_src_color,
		/// The parameter is multiplied by the source alpha.
		src_alpha,
		/// The parameter is multiplied by 1 - the source alpha.
		one_minus_src_alpha,
		/// The parameter is multiplied by the destination alpha.
		dst_alpha,
		/// The parameter is multiplied by 1 - the destination alpha.
		one_minus_dst_alpha,
		/// The parameter is multiplied by the destination color.
		dst_color,
		/// The parameter is multiplied by 1 - the destination color.
		one_minus_dst_color
	};

	/// Blending mode information.
	struct blend_mode
	{
		// Multiplier used for the source color.
		blend_multiplier rgb_src;

		// Operation applied with the source and destination colors.
		blend_fn rgb_fn;

		// Multiplier used for the destination color.
		blend_multiplier rgb_dst;

		// Multiplier used for the source alpha.
		blend_multiplier alpha_src;

		// Operation applied with the source and destination alpha.
		blend_fn alpha_fn;

		// Multiplier used for the destination alpha.
		blend_multiplier alpha_dst;

		//

		/// Compares two blending modes for equality.
		/// @param lhs, rhs Blending mode to compare.
		/// @return Whether the blending modes are equal.
		friend bool operator==(const blend_mode& lhs, const blend_mode& rhs) = default;
	};

	/// @name Blending modes
	/// @{

	/// Alpha blending mode.
	inline constexpr blend_mode alpha_blending{blend_multiplier::src_alpha, blend_fn::add, blend_multiplier::one_minus_src_alpha,
											   blend_multiplier::one,       blend_fn::add, blend_multiplier::one_minus_src_alpha};
	/// Premultiplied alpha blending mode.
	inline constexpr blend_mode premultiplied_alpha_blending{blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha,
															 blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha};
	/// Writes the maximum of two values into the buffer.
	inline constexpr blend_mode max_blending{blend_multiplier::one, blend_fn::max, blend_multiplier::one,
											 blend_multiplier::one, blend_fn::max, blend_multiplier::one};
	/// Applied after MAX_BLENDING to achieve normal alpha blending.
	inline constexpr blend_mode reverse_alpha_blending{blend_multiplier::one_minus_dst_alpha, blend_fn::add, blend_multiplier::dst_alpha,
													   blend_multiplier::one_minus_dst_alpha, blend_fn::max, blend_multiplier::dst_alpha};

	/// @}
} // namespace tr