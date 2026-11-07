/// @file
/// @brief Implements binary_io.hpp.

#include <tr/utility/binary_io.hpp>

//

tr::binary_read_error::binary_read_error(usize start_position, usize expected_bytes, usize read_bytes)
	: m_start_position{start_position}
	, m_expected_bytes{expected_bytes}
	, m_read_bytes{read_bytes}
	, m_description{std::format("Expected to read {} bytes from position {}, read {} instead.", expected_bytes, start_position, read_bytes)}
{
}

std::string_view tr::binary_read_error::name() const noexcept
{
	return "Binary read error";
}

std::string_view tr::binary_read_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::binary_read_error::details() const noexcept
{
	return std::string_view{};
}

tr::usize tr::binary_read_error::start_position() const noexcept
{
	return m_start_position;
}

tr::usize tr::binary_read_error::expected_bytes() const noexcept
{
	return m_expected_bytes;
}

tr::usize tr::binary_read_error::read_bytes() const noexcept
{
	return m_read_bytes;
}

//

tr::binary_write_error::binary_write_error(usize start_position, usize expected_bytes, usize written_bytes)
	: m_start_position{start_position}
	, m_expected_bytes{expected_bytes}
	, m_written_bytes{written_bytes}
	, m_description{
		  std::format("Expected to write {} bytes from position {}, wrote {} instead.", expected_bytes, start_position, written_bytes),
	  }
{
}

std::string_view tr::binary_write_error::name() const noexcept
{
	return "Binary write error";
}

std::string_view tr::binary_write_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::binary_write_error::details() const noexcept
{
	return std::string_view{};
}

tr::usize tr::binary_write_error::start_position() const noexcept
{
	return m_start_position;
}

tr::usize tr::binary_write_error::expected_bytes() const noexcept
{
	return m_expected_bytes;
}

tr::usize tr::binary_write_error::written_bytes() const noexcept
{
	return m_written_bytes;
}