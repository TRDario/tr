/// @file
/// @brief Implements binary_sink.hpp.

#include <tr/utility/binary_sink.hpp>

//

tr::file_binary_sink::file_binary_sink(const std::filesystem::path& path)
	: m_file{nullptr}
	, m_position{0}
{
#ifdef _WIN32
	if (!(m_file = _wfopen(path.c_str(), L"wb")))
#else
	if (!(m_file = std::fopen(path.c_str(), "wb")))
#endif
	{
		throw file_open_error{path.string()};
	}
}

tr::file_binary_sink::file_binary_sink(file_binary_sink&& rhs) noexcept
	: m_file{std::exchange(rhs.m_file, nullptr)}
	, m_position{std::exchange(rhs.m_position, 0)}
{
}

tr::file_binary_sink& tr::file_binary_sink::operator=(file_binary_sink&& rhs) noexcept
{
	if (m_file) {
		std::fclose(m_file);
	}
	m_file = std::exchange(rhs.m_file, nullptr);
	m_position = std::exchange(rhs.m_position, 0);
	return *this;
}

tr::file_binary_sink::~file_binary_sink() noexcept
{
	if (m_file) {
		std::fclose(m_file);
	}
}

//

tr::usize tr::file_binary_sink::write(std::span<const std::byte> source) noexcept
{
	if (!m_file) {
		return 0;
	}

	const usize written_bytes{std::fwrite(source.data(), 1, source.size(), m_file)};
	m_position += written_bytes;
	return written_bytes;
}

tr::usize tr::file_binary_sink::position() const noexcept
{
	return m_position;
}

//

tr::file_binary_sink::operator bool() const noexcept
{
	return m_file;
}

bool tr::file_binary_sink::is_open() const noexcept
{
	return m_file;
}

void tr::file_binary_sink::open(const std::filesystem::path& path)
{
#ifdef _WIN32
	std::FILE* const file{_wfopen(path.c_str(), L"wb")};
#else
	std::FILE* const file{std::fopen(path.c_str(), "wb")};
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

void tr::file_binary_sink::close() noexcept
{
	if (m_file) {
		std::fclose(m_file);
		m_file = nullptr;
		m_position = 0;
	}
}