///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements exception.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/exception.hpp"

//////////////////////////////////////////////////////////////// EXCEPTIONS ///////////////////////////////////////////////////////////////

thread_local tr::static_string<1024> tr::exception::g_what_buffer{1024, '\0'};

const char* tr::exception::what() const noexcept
{
	g_what_buffer.clear();

	TR_FMT::format_to_n(std::back_inserter(g_what_buffer), g_what_buffer.max_size() - 1, "{}: {}", name(), description());
	if (!details().empty()) {
		TR_FMT::format_to_n(std::back_inserter(g_what_buffer), g_what_buffer.max_size() - 1 - g_what_buffer.size(), " ({})", details());
	}
	g_what_buffer.append('\0');
	return g_what_buffer.data();
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

tr::custom_exception::custom_exception(std::string&& name, std::string&& description, std::string&& details)
	: m_name{std::move(name)}, m_description{std::move(description)}, m_details{std::move(details)}
{
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