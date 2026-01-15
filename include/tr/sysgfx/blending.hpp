///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides blending datatypes and constants.                                                                                            //
//                                                                                                                                       //
// tr always uses separated rgb and alpha blending modes, so blending modes consist of two sets of (function, multiplier, function).     //
//     - blend_mode alpha_blending{                                                                                                      //
//           tr::sys::blend_multiplier::src_alpha,                                                                                       //
//           tr::sys::blend_fn::add,                                                                                                     //
//           tr::sys::blend_multiplier::one_minus_src_alpha,                                                                             //
//           tr::sys::blend_multiplier::one,                                                                                             //
//           tr::sys::blend_fn::add,                                                                                                     //
//           tr::sys::blend_multiplier::one_minus_src_alpha                                                                              //
//       }                                                                                                                               //
//                                                                                                                                       //
// Alpha blending, premultiplied alpha blending, maximum blending and reverse alpha blending are provided as constants.                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Blending functions.
	enum class blend_fn {
		add = 0x8006,      // The source and destination colors are added together.
		min,               // The minimum channel values of the source and destination colors are selected.
		max,               // The maximum channel values of the source and destination colors are selected.
		subtract = 0x800A, // The source and destination colors are subtracted in the form (src - dst).
		reverse_subtract   // The source and destination colors are subtracted in the form (dst - src).
	};

	// Blending parameter multipliers.
	enum class blend_multiplier {
		zero,                // The parameter is multiplied by 0.
		one,                 // The parameter is multiplied by 1.
		src_color = 0x300,   // The parameter is multiplied by the source color.
		one_minus_src_color, // The parameter is multiplied by 1 - the source color.
		src_alpha,           // The parameter is multiplied by the source alpha.
		one_minus_src_alpha, // The parameter is multiplied by 1 - the source alpha.
		dst_alpha,           // The parameter is multiplied by the destination alpha.
		one_minus_dst_alpha, // The parameter is multiplied by 1 - the destination alpha.
		dst_color,           // The parameter is multiplied by the destination color.
		one_minus_dst_color  // The parameter is multiplied by 1 - the destination color.
	};

	// Blending mode information.
	struct blend_mode {
		// The multiplier used for the source color.
		blend_multiplier rgb_src;
		// The operation applied with the source and destination colors.
		blend_fn rgb_fn;
		// The multiplier used for the destination color.
		blend_multiplier rgb_dst;
		// The multiplier used for the source alpha.
		blend_multiplier alpha_src;
		// The operation applied with the source and destination alpha.
		blend_fn alpha_fn;
		// The multiplier used for the destination alpha.
		blend_multiplier alpha_dst;

		friend bool operator==(const blend_mode& l, const blend_mode& r) = default;
	};

	// Alpha blending mode.
	inline constexpr blend_mode alpha_blending{blend_multiplier::src_alpha, blend_fn::add, blend_multiplier::one_minus_src_alpha,
											   blend_multiplier::one,       blend_fn::add, blend_multiplier::one_minus_src_alpha};
	// Premultiplied alpha blending mode.
	inline constexpr blend_mode premultiplied_alpha_blending{blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha,
															 blend_multiplier::one, blend_fn::add, blend_multiplier::one_minus_src_alpha};
	// Writes the maximum of two values into the buffer.
	inline constexpr blend_mode max_blending{blend_multiplier::one, blend_fn::max, blend_multiplier::one,
											 blend_multiplier::one, blend_fn::max, blend_multiplier::one};
	// Applied after MAX_BLENDING to achieve normal alpha blending.
	inline constexpr blend_mode reverse_alpha_blending{blend_multiplier::one_minus_dst_alpha, blend_fn::add, blend_multiplier::dst_alpha,
													   blend_multiplier::one_minus_dst_alpha, blend_fn::max, blend_multiplier::dst_alpha};
} // namespace tr::gfx