#include "../../include/tr/utility/iostream.hpp"

tr::file_not_found::file_not_found(std::string_view path) noexcept
	: _path{std::format("'{}'", path)}
{
}

std::string_view tr::file_not_found::name() const noexcept
{
	return "File not found";
}

std::string_view tr::file_not_found::description() const noexcept
{
	return _path;
}

std::string_view tr::file_not_found::details() const noexcept
{
	return {};
}

tr::file_open_error::file_open_error(std::string_view path) noexcept
	: _path{std::format("'{}'", path)}
{
}

std::string_view tr::file_open_error::name() const noexcept
{
	return "File opening error";
}

std::string_view tr::file_open_error::description() const noexcept
{
	static std::string buffer;
	buffer = std::format("'{}'", _path);
	return buffer;
}

std::string_view tr::file_open_error::details() const noexcept
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