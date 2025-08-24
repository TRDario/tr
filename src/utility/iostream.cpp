#include "../../include/tr/utility/iostream.hpp"

tr::file_not_found::file_not_found(std::string_view path)
	: m_path{TR_FMT::format("'{}'", path)}
{
}

std::string_view tr::file_not_found::name() const
{
	return "File not found";
}

std::string_view tr::file_not_found::description() const
{
	return m_path;
}

std::string_view tr::file_not_found::details() const
{
	return {};
}

tr::file_open_error::file_open_error(std::string_view path)
	: m_path{TR_FMT::format("'{}'", path)}
{
}

std::string_view tr::file_open_error::name() const
{
	return "File opening error";
}

std::string_view tr::file_open_error::description() const
{
	static std::string buffer;
	buffer = TR_FMT::format("'{}'", m_path);
	return buffer;
}

std::string_view tr::file_open_error::details() const
{
	return {};
}

std::ofstream tr::open_file_w(const std::filesystem::path& path, std::ios::openmode openmode)
{
	std::ofstream file{path, openmode};
	if (!file.is_open()) {
		throw file_not_found{path.string()};
	}
	file.exceptions(std::ios::badbit | std::ios::failbit);
	return file;
}

std::ifstream tr::open_file_r(const std::filesystem::path& path, std::ios::openmode openmode)
{
	if (!is_regular_file(path)) {
		throw file_not_found{path.string()};
	}
	std::ifstream file{path, openmode};
	if (!file.is_open()) {
		throw file_open_error{path.string()};
	}
	file.exceptions(std::ios::badbit | std::ios::failbit);
	return file;
}