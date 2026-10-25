/// @file
/// @brief Implements color_hex_code.hpp.

#include <tr/utility/color_hex_code.hpp>

//

tr::invalid_hex_code::invalid_hex_code(std::string_view hex_code, std::string&& details)
	: m_hex_code{std::string{hex_code}}
	, m_details{std::move(details)}
{
}

std::string_view tr::invalid_hex_code::name() const noexcept
{
	return "Invalid hex code string";
}

std::string_view tr::invalid_hex_code::description() const noexcept
{
	return m_hex_code;
}

std::string_view tr::invalid_hex_code::details() const noexcept
{
	return m_details;
}