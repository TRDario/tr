#pragma once
#include "../utility/exception.hpp"

namespace tr {
	// Error thrown when system initialization fails.
	class system_initialization_error : public tr::exception {
	  public:
		// Constructs an exception.
		system_initialization_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string_view description_str;
	};

	// Sets up the basic application info.
	// May throw: system_initialization_error.
	void initialize_application(const char* developer, const char* name);
} // namespace tr