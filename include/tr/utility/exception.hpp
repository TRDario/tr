///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides various basic exception types.                                                                                               //
//                                                                                                                                       //
// tr::exception provides an interface for tr-style exceptions, which instead of having a single "what" message can consist of up to     //
// three: name, description, and details. The actual .what() method combines these into the format "<NAME>: <DESCRIPTION> (<DETAILS>)":  //
//     - tr::custom_exception{"foo error", "bar not found", "baz"}.what() -> "foo error: bar not found (baz)"                            //
// NOTE: tr::exception uses a single statically allocated buffer for what() string, so it is not possible to have multiple what()        //
// messages at once.                                                                                                                     //
//                                                                                                                                       //
// tr::out_of_memory is a custom out-of-memory exception whose description can be formatted, while tr::custom_exception forwards the     //
// strings it is passed as the name/description/details strings:                                                                         //
//     - tr::out_of_memory{"bitmap allocation"}.what() -> "Out of memory error: During bitmap allocation."                               //
//     - tr::custom_exception{"foo error", "bar not found", "baz"}.what() -> "foo error: bar not found (baz)"                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "static_string.hpp"
#include <iterator>

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Extension of std::exception.
	class exception : public std::exception {
	  public:
		// Gets the name of the error.
		virtual std::string_view name() const = 0;
		// Gets the description of the error.
		virtual std::string_view description() const = 0;
		// Gets further details about the error.
		virtual std::string_view details() const = 0;
		// Gets a formatted error message.
		const char* what() const noexcept final;

	  private:
		// Buffer used for exception explanation messages.
		static thread_local static_string<1024> g_what_buffer;
	};

	// Custom out-of-memory exception.
	struct out_of_memory : public exception {
		// Constructs an out-of-memory exception (arguments are formatted in-place).
		template <class... Args> out_of_memory(TR_FORMAT_STRING<Args...> fmt, Args&&... args);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		static_string<255> m_description;
	};

	// Specialization of tr::exception for one-off exceptions.
	class custom_exception : public exception {
	  public:
		// Constructs an exception.
		custom_exception(std::string&& name, std::string&& description, std::string&& details);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The name of the error.
		std::string m_name;
		// The description of the error.
		std::string m_description;
		// Further details about the error.
		std::string m_details;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class... Args> tr::out_of_memory::out_of_memory(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	auto it{std::back_inserter(m_description)};
	TR_FMT::format_to(it, "During ");
	TR_FMT::format_to_n(it, m_description.max_size() - 2 - m_description.size(), fmt, std::forward<Args>(args)...);
	TR_FMT::format_to(it, ".");
}