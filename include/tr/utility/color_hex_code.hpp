/// @file
/// @brief Provides `tr::from_hex_code()` and hex code color literals.

/// @ingroup color
/// @defgroup color_hex_code Hex codes
/// @brief Contains `tr::from_hex_code()` and hex code color literals.

#pragma once
#include <tr/utility/color_cast.hpp>
#include <tr/utility/exception.hpp>
#include <tr/utility/rgb.hpp>
#include <tr/utility/rgba.hpp>

//

namespace tr
{
	/// @ingroup color_hex_code
	/// @brief Exception thrown for an invalid hex code string in `tr::from_hex_code()`.
	class invalid_hex_code : public exception
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an invalid hex code exception.
		/// @param hex_code Hex code string.
		/// @param details String containing further details about the error.
		[[nodiscard]] invalid_hex_code(std::string_view hex_code, std::string&& details);

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the name of the error.
		/// @return Always `Invalid hex code string`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// @brief Returns the description of the error.
		/// @return String containing the error description.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// @brief Returns further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// @brief Invalid hex code string.
		std::string m_hex_code;

		/// @brief View to the details string.
		std::string m_details;
	};

	//

	/// @ingroup color_hex_code
	/// @brief Creates a color value from a hex code string.
	/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
	/// @tparam Color Type of the created color.
	/// @param hex_code Hex code string.
	/// @exception invalid_hex_code If the hex code string is invalid.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Color value equivalent to the hex code string.
	template <typename Color = rgba<u8>>
		requires indirectly_color_castable_to<rgba<u8>, Color>
	[[nodiscard]] constexpr Color from_hex_code(std::string_view hex_code)
	{
		if (hex_code.size() != 7 && hex_code.size() != 9) {
			throw invalid_hex_code{hex_code, std::format("must be 7 or 9 characters long, but is {}", hex_code.size())};
		}
		else if (hex_code[0] != '#') {
			throw invalid_hex_code{hex_code, std::format("must start with '#', but starts with '{}'", hex_code[0])};
		}
		for (usize i = 1; i < hex_code.size(); ++i) {
			const char c{hex_code[i]};
			if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F')) {
				throw invalid_hex_code{hex_code, std::format("contains invalid hex character '{}' at position {}", c, i)};
			}
		}

		constexpr auto hex_value{[](char c) -> u8 {
			if (c >= '0' && c <= '9') {
				return c - '0';
			}
			else if (c >= 'a' && c <= 'f') {
				return c - 'a' + 10;
			}
			else /* c >= 'A' && c <= 'F' */ {
				return c - 'A' + 10;
			}
		}};

		return color_cast<Color>(rgba<u8>{
			static_cast<u8>((hex_value(hex_code[1]) << 4) | hex_value(hex_code[2])),
			static_cast<u8>((hex_value(hex_code[3]) << 4) | hex_value(hex_code[4])),
			static_cast<u8>((hex_value(hex_code[5]) << 4) | hex_value(hex_code[6])),
			static_cast<u8>(hex_code.size() == 9 ? (hex_value(hex_code[7]) << 4) | hex_value(hex_code[8]) : 255),
		});
	}

	//

	inline namespace literals
	{
		/// @ingroup color_hex_code
		/// @brief Contains hex code color literals.
		inline namespace color_literals
		{
			/// @brief Returns an 8-bit RGB color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGB color.
			[[nodiscard]] consteval rgb<u8> operator""_rgb8(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgb<u8>>(std::string_view{hex_code, length});
			}

			/// @brief Returns a float RGB color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding float RGB color.
			[[nodiscard]] consteval rgb<float> operator""_rgbf(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgb<float>>(std::string_view{hex_code, length});
			}

			/// @brief Returns a double RGB color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding double RGB color.
			[[nodiscard]] consteval rgb<double> operator""_rgb(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgb<double>>(std::string_view{hex_code, length});
			}

			/// @brief Returns an 8-bit RGBA color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGBA color.
			[[nodiscard]] consteval rgba<u8> operator""_rgba8(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgba<u8>>(std::string_view{hex_code, length});
			}

			/// @brief Returns a float RGBA color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding float RGBA color.
			[[nodiscard]] consteval rgba<float> operator""_rgbaf(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgba<float>>(std::string_view{hex_code, length});
			}

			/// @brief Returns a double RGBA color value equivalent to a hex code string.
			/// @details `hex_code` must be in the format %"#XXYYZZ" or %"#XXYYZZWW" with no whitespace to be considered valid.
			/// @param hex_code Color hex code string.
			/// @param length Length of the hex string.
			/// @return Corresponding double RGBA color.
			[[nodiscard]] consteval rgba<double> operator""_rgba(const char* hex_code, usize length) noexcept
			{
				return from_hex_code<rgba<double>>(std::string_view{hex_code, length});
			}
		} // namespace color_literals
	} // namespace literals
} // namespace tr