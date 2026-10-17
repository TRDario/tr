/// @file
/// @brief Implements temporary_file.hpp.

#include <tr/utility/temporary_file.hpp>

//

tr::temporary_file::temporary_file(std::string_view filename_base, std::string_view extension)
	: m_path{std::filesystem::temp_directory_path() /
			 std::format("{}{}{}", filename_base, std::chrono::steady_clock::now().time_since_epoch().count(), extension)}
{
}

tr::temporary_file::temporary_file(temporary_file&& rhs) noexcept
	: m_path{std::move(rhs.m_path)}
{
	rhs.m_path.clear();
}

tr::temporary_file::~temporary_file() noexcept
{
	if (!m_path.empty()) {
		std::error_code error;
		std::filesystem::remove(m_path, error);
	}
}

//

tr::temporary_file& tr::temporary_file::operator=(temporary_file&& rhs) noexcept
{
	if (!m_path.empty()) {
		std::error_code error;
		std::filesystem::remove(m_path, error);
	}
	m_path = std::move(rhs.m_path);
	rhs.m_path.clear();
	return *this;
}

//

const std::filesystem::path& tr::temporary_file::path() const noexcept
{
	return m_path;
}