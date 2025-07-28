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

tr::custom_exception::custom_exception(std::string&& name, std::string&& description, std::string&& details)
	: name_str{std::move(name)}, description_str{std::move(description)}, details_str{std::move(details)}
{
}

std::string_view tr::custom_exception::name() const
{
	return name_str;
}

std::string_view tr::custom_exception::description() const
{
	return description_str;
}

std::string_view tr::custom_exception::details() const
{
	return details_str;
}