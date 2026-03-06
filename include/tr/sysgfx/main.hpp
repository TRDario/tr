///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functionality related to the main loop of the program.                                                                       //
//                                                                                                                                       //
// Unlike a standard C++ program, programs using tr do not have to define main(). Instead, a number of functions in the namespace        //
// tr::sys::main have to be defined, as well as tr::sys::main::metadata, a struct containing basic application metadata:                 //
//     - tr::sys::main::metadata{.name = "Example", .version = "v0", .developer = "Me"}                                                  //
//       -> example metadata struct definition                                                                                           //
//     - tr::sys::main::parse_command_line(std::span<cstring_view> args)                                                                 //
//       -> called before any system initialization, meant for parsing command-line arguments and other preinitialization                //
//     - tr::sys::main::initialize()                                                                                                     //
//       -> called after system initialization, meant to initialize the application state and open the window                            //
//     - tr::sys::main::handle_event(const event& event)                                                                                 //
//       -> called when an event is recieved, meant to handle the event                                                                  //
//     - tr::sys::main::update(duration delta)                                                                                           //
//       -> called at the rate defined in set_update_frequency (uncapped by default), meant for updating and drawing the application     //
//     - tr::sys::main::shut_down()                                                                                                      //
//       -> called after an exit signal is returned by one of the other main functions, meant to clean up the application state          //
//                                                                                                                                       //
// Most functions in tr::sys::main, with the exception of shut_down, return a signal. If tr::sys::signal::proceed is returned, execution //
// will continue as normal. If tr::sys::signal::exit or tr::sys::signal::abort is returned, further execution is stopped and shut_down   //
// is called to clean up the application state.                                                                                          //
//                                                                                                                                       //
// The rate at which tr::sys::main::update() is called can be set with set_update_frequency, and can be reset to the default behavior    //
// by setting it to tr::sys::uncapped_update_frequency:                                                                                  //
//     - tr::sys::set_update_frequency(60) -> tr::sys::main::update() will be called 60 times a second                                   //
//     - tr::sys::set_update_frequency(tr::sys::uncapped_update_frequency) -> tr::sys::main::update() will be called as often as possible//
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/exception.hpp"
#include "event.hpp"

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

	// Sentinel representing an uncapped update frequency (the default behavior).
	inline constexpr float uncapped_update_frequency{0};
	// Sets the frequency at which update() is called (by default uncapped).
	void set_update_frequency(float frequency);

	// User-defined functions and data (mandatory). Uncaught exceptions will display a dialog box and quit the application.
	namespace main {
		// Application metadata.
		extern const app_metadata metadata;

		// Called once at the beginning of execution, but before the initialization of any systems.
		signal parse_command_line(std::span<cstring_view> args);
		// Called once at the beginning of execution after the parsing of the command line arguments.
		signal initialize();
		// Called whenever an event needs to be handled. Not guaranteed to be called on the main thread.
		signal handle_event(const event& event);
		// Primary update function.
		signal update(duration delta);
		// Called once at the end of execution.
		void shut_down();
	} // namespace main
} // namespace tr::sys