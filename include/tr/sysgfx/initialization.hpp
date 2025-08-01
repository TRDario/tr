#pragma once
#include "../utility/exception.hpp"

namespace tr::system {
	// Error thrown when system initialization fails.
	class init_error : public tr::exception {
	  public:
		// Constructs an exception.
		init_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string_view m_description;
	};

	// Sets up the basic application info.
	// May throw: init_error.
	void initialize(const char* developer, const char* name);
} // namespace tr::system