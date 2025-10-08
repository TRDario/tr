#pragma once
#include "event_types.hpp"

namespace tr::sys {
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

	// Signals used for controlling control flow of the user defined callbacks.
	enum class signal {
		// Continue execution.
		CONTINUE,
		// Regular exit.
		SUCCESS,
		// Abnormal exit.
		FAILURE
	};

	// Sets various app information.
	void set_app_information(const char* developer, const char* name, const char* version);
	// Sets the frequency at which tr::sys::user_defined::iterate() is called.
	void set_iteration_frequency(float frequency);

	// User-defined functions (mandatory). Uncaught exception will display a dialog box and quit the application.
	namespace user_defined {
		// Called once at the beginning of execution.
		signal initialize(std::span<const char*> args);
		// Called whenever an event needs to be handled.
		signal handle_event(event& event);
		// Main-body function to be used for rendering and the like.
		signal iterate();
		// Called once at the end of execution.
		void quit();
	} // namespace user_defined
} // namespace tr::sys