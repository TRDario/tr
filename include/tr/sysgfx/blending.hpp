#pragma once

namespace tr::gfx {
	// Blending functions.
	enum class blend_fn {
		ADD = 0x8006,      // The source and destination colors are added together.
		MIN,               // The minimum channel values of the source and destination colors are selected.
		MAX,               // The maximum channel values of the source and destination colors are selected.
		SUBTRACT = 0x800A, // The source and destination colors are subtracted in the form (src - dst).
		REVERSE_SUBTRACT   // The source and destination colors are subtracted in the form (dst - src).
	};

	// Blending parameter multipliers.
	enum class blend_multiplier {
		ZERO,                // The parameter is multiplied by 0.
		ONE,                 // The parameter is multiplied by 1.
		SRC_COLOR = 0x300,   // The parameter is multiplied by the source color.
		ONE_MINUS_SRC_COLOR, // The parameter is multiplied by 1 - the source color.
		SRC_ALPHA,           // The parameter is multiplied by the source alpha.
		ONE_MINUS_SRC_ALPHA, // The parameter is multiplied by 1 - the source alpha.
		DST_ALPHA,           // The parameter is multiplied by the destination alpha.
		ONE_MINUS_DST_ALPHA, // The parameter is multiplied by 1 - the destination alpha.
		DST_COLOR,           // The parameter is multiplied by the destination color.
		ONE_MINUS_DST_COLOR  // The parameter is multiplied by 1 - the destination color.
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
	inline constexpr blend_mode ALPHA_BLENDING{blend_multiplier::SRC_ALPHA, blend_fn::ADD, blend_multiplier::ONE_MINUS_SRC_ALPHA,
											   blend_multiplier::ONE,       blend_fn::ADD, blend_multiplier::ONE_MINUS_SRC_ALPHA};

	// Premultiplied alpha blending mode.
	inline constexpr blend_mode PREMUL_ALPHA_BLENDING{blend_multiplier::ONE, blend_fn::ADD, blend_multiplier::ONE_MINUS_SRC_ALPHA,
													  blend_multiplier::ONE, blend_fn::ADD, blend_multiplier::ONE_MINUS_SRC_ALPHA};
} // namespace tr::gfx