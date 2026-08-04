/// @file
/// @brief Provides base exception types.

#pragma once
#include "static_string.hpp"

//

namespace tr
{
	/// Extension of `std::exception` and base class of tr errors.
	/// @details
	/// Instead of having a single "what" message, tr errors are defined with up to three message components: name, description, and
	/// details. The actual what() method is finalized and combines the three sections into a single message.
	class exception : public std::exception
	{
	  public:
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return Name of the error.
		virtual std::string_view name() const = 0;

		/// Gets the description of the error.
		/// @return String containing the error description, or empty to omit the description section from the message.
		virtual std::string_view description() const = 0;

		/// Gets further details about the error.
		/// @return String containing details about the error, or empty to omit the details section from the message.
		virtual std::string_view details() const = 0;

		/// Gets a formatted error message.
		/// @return Error message in the format `"<NAME>: <DESCRIPTION> (<DETAILS>)"`.
		const char* what() const noexcept final;

		/// @}

	  private:
		/// Buffer used for exception explanation messages.
		static thread_local static_string<1024> g_what_buffer;
	};

	/// Out-of-memory exception.
	struct out_of_memory : public exception
	{
		/// @name Constructors
		/// @{

		/// Constructs an out-of-memory exception (arguments are formatted in-place).
		/// @tparam Args Formatting argument types.
		/// @param fmt Error message format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		out_of_memory(std::format_string<Args...> fmt, Args&&... args);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `Out of memory`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the out-of-memory error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		static_string<255> m_description;
	};

	/// Specialization of tr::exception for one-off exceptions.
	class custom_exception : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param name Name of the error.
		/// @param description Description of the error.
		/// @param details Details of the error.
		custom_exception(std::string&& name, std::string&& description, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return Name of the error.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Further details about the error.
		std::string_view details() const override;

		/// @}

	  private:
		// The name of the error.
		std::string m_name;
		// The description of the error.
		std::string m_description;
		// Further details about the error.
		std::string m_details;
	};
} // namespace tr

#include "impl/exception.hpp" // IWYU pragma: export