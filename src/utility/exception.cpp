#include "../../include/tr/utility/exception.hpp"

namespace tr {
	// Buffer used for exception explanation messages.
	static_string<1024> what_buffer{1024, '\0'};
} // namespace tr

const char* tr::exception::what() const noexcept
{
	what_buffer.clear();
	std::format_to_n(std::back_inserter(what_buffer), what_buffer.max_size() - 1, "{}: {}", name(), description());
	if (!details().empty()) {
		std::format_to_n(std::back_inserter(what_buffer), what_buffer.max_size() - 1 - what_buffer.size(), " ({})", details());
	}
	return what_buffer.data();
}

tr::custom_exception::custom_exception(std::string&& name, std::string&& description, std::string&& details)
	: name_str{std::move(name)}, description_str{std::move(description)}, details_str{std::move(details)}
{
}

tr::out_of_memory::out_of_memory(std::string_view description)
{
	std::format_to_n(std::back_inserter(description_str), description_str.max_size(), "During {}.", description);
}

std::string_view tr::out_of_memory::name() const
{
	return "Out of memory error";
}

std::string_view tr::out_of_memory::description() const
{
	return description_str;
}

std::string_view tr::out_of_memory::details() const
{
	return {};
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