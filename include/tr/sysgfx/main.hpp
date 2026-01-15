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
		proceed, // Continue execution.
		exit,    // Regular exit.
		abort    // Abnormal exit.
	};

	// Supported application types.
	enum class app_type {
		game,       // The application is a game.
		application // The application type is unspecified.
	};

	// Application metadata.
	struct app_metadata {
		// The name of the application.
		cstring_view name{};
		// The version of the application.
		cstring_view version{};
		// The identifier of the application.
		cstring_view identifier{};
		// The developer of the application.
		cstring_view developer{};
		// A short copyright notice.
		cstring_view copyright{};
		// A URL relevant to the application.
		cstring_view url{};
		// The type of the application.
		app_type type{app_type::application};
	};

	// Sets application metadata.
	void set_app_metadata(const app_metadata& metadata);
	// Sets the frequency at which tick() is called (by default not active).
	void set_tick_frequency(float frequency);
	// Sets the frequency at which draw() is called (by default set to the refresh rate).
	void set_draw_frequency(float frequency);
} // namespace tr::sys

/////////////// User-defined functions (mandatory). Uncaught exceptions will display a dialog box and quit the application. ///////////////

// Called once at the beginning of execution, but before the initialization of any systems.
tr::sys::signal parse_command_line(std::span<tr::cstring_view> args);
// Called once at the beginning of execution after the parsing of the command line arguments.
tr::sys::signal initialize();
// Called whenever an event needs to be handled.
tr::sys::signal handle_event(tr::sys::event& event);
// Main-body function used for game state ticks.
tr::sys::signal tick();
// Main-body function used for drawing and related updated.
tr::sys::signal draw();
// Called once at the end of execution.
void shut_down();