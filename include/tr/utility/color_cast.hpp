#pragma once
#include "color.hpp"

namespace tr {
	/** @addtogroup color
	 *  @{
	 */

	/******************************************************************************************************************
	 * Interface for custom casters for use in color_cast.
	 *
	 * To fulfill the requirements for a valid color caster specialization, the following must be present:
	 *
	 * static function to_built_in that takes a value/const reference of type @em T and returns a builtin color type.
	 *
	 * static function from_built_in that takes a value/const reference of a built_in_color type and returns a value
	 * of type <em>T</em>.
	 *
	 * If these conditions are properly met, @em T will fulfill color_castable_from and color_castable_to.
	 ******************************************************************************************************************/
	template <class T> struct color_caster {};

	/******************************************************************************************************************
	 * Concept that denotes a type able to be color casted from.
	 *
	 * To fulfill this condition, color_caster<T> must have a static function @em to_built_in taking a @em T
	 * and returning a builtin color type.
	 *******************************************************************************************************************/
	template <class T>
	concept color_castable_from = built_in_color<std::remove_cvref_t<return_type_t<decltype(color_caster<T>::to_built_in)>>>;

	/******************************************************************************************************************
	 * Concept that denotes a type able to be color casted to.
	 *
	 * To fulfill this condition, color_caster<T> must have a static function @em from_built_in taking a built_in_color
	 * and returning <em>T</em>.
	 ******************************************************************************************************************/
	template <class T>
	concept color_castable_to = built_in_color<std::remove_cvref_t<arg_type_t<decltype(color_caster<T>::from_built_in)>>> &&
								std::same_as<std::remove_cvref_t<return_type_t<decltype(color_caster<T>::from_built_in)>>, T>;

	/******************************************************************************************************************
	 * Converts a red channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of R.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, specialization_of<tr::r> From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a green channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of G.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, specialization_of<tr::g> From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a blue channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of B.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, specialization_of<tr::b> From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts an RG channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RG.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, specialization_of<tr::rg> From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a three-channel (RGB/BGR) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGB or BGR.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, class From>
		requires specialization_of<From, tr::rgb> || specialization_of<From, tr::bgr>
	constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a four-channel (RGBA/BGRA) channel color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A specialization of RGBA or BGRA.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, class From>
		requires specialization_of<From, tr::rgba> || specialization_of<From, tr::bgra>
	constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a RGB_Ui8_2_3_3 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const rgb8_223& from);

	/******************************************************************************************************************
	 * Converts a RGB_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const rgb16_565& from);

	/******************************************************************************************************************
	 * Converts a BGR_Ui8_3_3_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const bgr8_332& from);

	/******************************************************************************************************************
	 * Converts a BGR_Ui16_5_6_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const bgr16_565& from);

	/******************************************************************************************************************
	 * Converts a RGBA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const rgba16_4444& from);

	/******************************************************************************************************************
	 * Converts a RGBA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const rgba16_5551& from);

	/******************************************************************************************************************
	 * Converts a RGBA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const rgba32_1010102& from);

	/******************************************************************************************************************
	 * Converts a ABGR_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const abgr16_4444& from);

	/******************************************************************************************************************
	 * Converts a ABGR_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const abgr16_1555& from);

	/******************************************************************************************************************
	 * Converts a ABGR_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const abgr32_2101010& from);

	/******************************************************************************************************************
	 * Converts a BGRA_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const bgra16_4444& from);

	/******************************************************************************************************************
	 * Converts a BGRA_Ui16_5_5_5_1 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const bgra16_5551& from);

	/******************************************************************************************************************
	 * Converts a BGRA_Ui32_10_10_10_2 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const bgra32_1010102& from);

	/******************************************************************************************************************
	 * Converts a ARGB_Ui16_4_4_4_4 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const argb16_4444& from);

	/******************************************************************************************************************
	 * Converts a ARGB_Ui16_1_5_5_5 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const argb16_1555& from);

	/******************************************************************************************************************
	 * Converts a ARGB_Ui32_2_10_10_10 color to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To> constexpr To color_cast(const argb32_2101010& from);

	/******************************************************************************************************************
	 * Converts a custom castable color type to a built-in color.
	 *
	 * @tparam To A built-in color type.
	 * @tparam From A type with a specialized color_caster.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <built_in_color To, color_castable_from From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a built-in color to a custom castable color type.
	 *
	 * @tparam To A type with a specialized color_caster.
	 * @tparam From A built-in color type.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <color_castable_to To, built_in_color From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Converts a custom castable color type to another custom castable color type.
	 *
	 * @tparam To A type with a specialized color_caster.
	 * @tparam From A type with a specialized color_caster.
	 *
	 * @param[in] from The value to be cast.
	 *
	 * @return The converted value.
	 ******************************************************************************************************************/
	template <color_castable_to To, color_castable_from From> constexpr To color_cast(const From& from);

	/******************************************************************************************************************
	 * Specialization of color_caster for HSV.
	 ******************************************************************************************************************/
	template <> struct color_caster<hsv> {
		/******************************************************************************************************************
		 * Casts an HSV value to a floating point RGBA value.
		 *
		 * @param[in] from The HSV value to be converted.
		 *
		 * @return The corresponding RGBAF value.
		 ******************************************************************************************************************/
		static constexpr rgbaf to_built_in(const hsv& from);

		/******************************************************************************************************************
		 * Casts an RGBAF value to an HSV value.
		 *
		 * @param[in] from The RGBAF value to be converted.
		 *
		 * @return The corresponding HSV value.
		 ******************************************************************************************************************/
		static constexpr hsv from_built_in(const rgbaf& from);
	};

	/// @}
} // namespace tr

#include "color_cast_impl.hpp"