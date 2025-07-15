#include "../../include/tr/utility/exception.hpp"

const char* tr::exception::what() const noexcept
{
	static std::string buffer;
	if (details().empty()) {
		buffer = std::format("{}: {}", name(), description());
	}
	else {
		buffer = std::format("{}: {} ({})", name(), description(), details());
	}
	return buffer.c_str();
}

tr::custom_exception::custom_exception(std::string&& name, std::string&& description, std::string&& details) noexcept
	: _name{std::move(name)}, _description{std::move(description)}, _details{std::move(details)}
{
}

std::string_view tr::custom_exception::name() const noexcept
{
	return _name;
}

std::string_view tr::custom_exception::description() const noexcept
{
	return _description;
}

std::string_view tr::custom_exception::details() const noexcept
{
	return _details;
}