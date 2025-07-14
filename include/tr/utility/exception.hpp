#pragma once
#include "common.hpp"

namespace tr {
	// Extension of std::exception.
	class exception : public std::exception {
	  public:
		// Constructs an exception.
		exception(std::string_view location) noexcept;

		// Gets the name of the error.
		virtual std::string_view name() const noexcept = 0;
		// Gets the description of the error.
		virtual std::string_view description() const noexcept = 0;
		// Gets further details about the error.
		virtual std::string_view details() const noexcept = 0;
		// Gets the location of the exception.
		std::string_view location() const noexcept;
		// Gets a formatted error message.
		const char* what() const noexcept final;

	  private:
		// The location where the exception occured.
		std::string_view _location;
	};
	// Specialization of tr::exception for one-off exceptions.
	class custom_exception : public exception {
	  public:
		// Constructs an exception.
		custom_exception(std::string_view location, std::string&& name, std::string&& description, std::string&& details) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The name of the error.
		std::string _name;
		// The description of the error.
		std::string _description;
		// Further details about the error.
		std::string _details;
	};
} // namespace tr