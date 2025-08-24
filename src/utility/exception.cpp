#include "../../include/tr/utility/exception.hpp"

namespace tr {
	// Buffer used for exception explanation messages.
	static_string<1024> what_buffer{1024, '\0'};
} // namespace tr

const char* tr::exception::what() const noexcept
{
	what_buffer.clear();
	TR_FMT::format_to_n(std::back_inserter(what_buffer), what_buffer.max_size() - 1, "{}: {}", name(), description());
	if (!details().empty()) {
		TR_FMT::format_to_n(std::back_inserter(what_buffer), what_buffer.max_size() - 1 - what_buffer.size(), " ({})", details());
	}
	return what_buffer.data();
}

tr::custom_exception::custom_exception(std::string&& name, std::string&& description, std::string&& details)
	: m_name{std::move(name)}, m_description{std::move(description)}, m_details{std::move(details)}
{
}

tr::out_of_memory::out_of_memory(std::string_view description)
{
	TR_FMT::format_to_n(std::back_inserter(m_description), m_description.max_size(), "During {}.", description);
}

std::string_view tr::out_of_memory::name() const
{
	return "Out of memory error";
}

std::string_view tr::out_of_memory::description() const
{
	return m_description;
}

std::string_view tr::out_of_memory::details() const
{
	return {};
}

std::string_view tr::custom_exception::name() const
{
	return m_name;
}

std::string_view tr::custom_exception::description() const
{
	return m_description;
}

std::string_view tr::custom_exception::details() const
{
	return m_details;
}