///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of exception.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../exception.hpp"

////////////////////////////////////////////////////////////// OUT OF MEMORY //////////////////////////////////////////////////////////////

template <typename... Args> tr::out_of_memory::out_of_memory(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	auto output_it{std::back_inserter(m_description)};
	TR_FMT::format_to(output_it, "Error occurred during ");
	TR_FMT::format_to_n(output_it, m_description.max_size() - 2 - m_description.size(), fmt, std::forward<Args>(args)...);
	TR_FMT::format_to(output_it, ".");
}