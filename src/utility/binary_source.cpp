/// @file
/// @brief Implements binary_source.hpp.

#include <tr/utility/binary_source.hpp>

//

tr::file_binary_source::file_binary_source(const std::filesystem::path& path)
	: m_file{nullptr}
	, m_position{0}
{
	if (!std::filesystem::is_regular_file(path)) {
		throw file_not_found{path.string()};
	}

#ifdef _WIN32
	if (!(m_file = _wfopen(path.c_str(), L"rb")))
#else
	if (!(m_file = std::fopen(path.c_str(), "rb")))
#endif
	{
		throw file_open_error{path.string()};
	}
}

tr::file_binary_source::file_binary_source(file_binary_source&& rhs) noexcept
	: m_file{std::exchange(rhs.m_file, nullptr)}
	, m_position{std::exchange(rhs.m_position, 0)}
{
}

tr::file_binary_source& tr::file_binary_source::operator=(file_binary_source&& rhs) noexcept
{
	if (m_file) {
		std::fclose(m_file);
	}
	m_file = std::exchange(rhs.m_file, nullptr);
	m_position = std::exchange(rhs.m_position, 0);
	return *this;
}

tr::file_binary_source::~file_binary_source() noexcept
{
	if (m_file) {
		std::fclose(m_file);
	}
}

//

[[nodiscard]] tr::usize tr::file_binary_source::read(std::span<std::byte> destination) noexcept
{
	if (!m_file) {
		return 0;
	}

	const usize bytes_read{std::fread(destination.data(), 1, destination.size(), m_file)};
	m_position += bytes_read;
	return bytes_read;
}

[[nodiscard]] tr::usize tr::file_binary_source::position() const noexcept
{
	return m_position;
}

//

tr::file_binary_source::operator bool() const noexcept
{
	return m_file;
}

bool tr::file_binary_source::is_open() const noexcept
{
	return m_file;
}

void tr::file_binary_source::open(const std::filesystem::path& path)
{
	if (!std::filesystem::is_regular_file(path)) {
		throw file_not_found{path.string()};
	}

#ifdef _WIN32
	std::FILE* const file{_wfopen(path.c_str(), L"rb")};
#else
	std::FILE* const file{std::fopen(path.c_str(), "rb")};
#endif
	if (!file) {
		throw file_open_error{path.string()};
	}

	if (m_file) {
		std::fclose(m_file);
	}
	m_file = file;
	m_position = 0;
}

void tr::file_binary_source::close() noexcept
{
	if (m_file) {
		std::fclose(m_file);
		m_file = nullptr;
		m_position = 0;
	}
}