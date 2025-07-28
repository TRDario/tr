#pragma once
#include "common.hpp"

namespace tr {
	// Extension of std::exception.
	struct exception : public std::exception {
		// Gets the name of the error.
		virtual std::string_view name() const = 0;
		// Gets the description of the error.
		virtual std::string_view description() const = 0;
		// Gets further details about the error.
		virtual std::string_view details() const = 0;
		// Gets a formatted error message.
		const char* what() const noexcept final;
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
		std::string name_str;
		// The description of the error.
		std::string description_str;
		// Further details about the error.
		std::string details_str;
	};
} // namespace tr