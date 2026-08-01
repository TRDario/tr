///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of exception.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../exception.hpp"

////////////////////////////////////////////////////////////// OUT OF MEMORY //////////////////////////////////////////////////////////////

template <typename... Args> tr::out_of_memory::out_of_memory(std::format_string<Args...> fmt, Args&&... args)
{
	auto output_it{std::back_inserter(m_description)};
	std::format_to(output_it, "Error occurred during ");
	std::format_to_n(output_it, m_description.max_size() - 2 - m_description.size(), fmt, std::forward<Args>(args)...);
	std::format_to(output_it, ".");
}