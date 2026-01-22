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
//     - tr::sys::main::tick()                                                                                                           //
//       -> called at the rate defined in set_tick_frequency (by default not active), meant for fixed-rate updates                       //
//     - tr::sys::main::draw()                                                                                                           //
//       -> called at the rate defined in set_draw_frequency (the refresh rate, by default), meant for delta-time updates and drawing    //
//     - tr::sys::main::shut_down()                                                                                                      //
//       -> called after an exit signal is returned by one of the other main functions, meant to clean up the application state          //
//                                                                                                                                       //
// Most functions in tr::sys::main, with the exception of shut_down, return a signal. If tr::sys::signal::proceed is returned, execution //
// will continue as normal. If tr::sys::signal::exit or tr::sys::signal::abort is returned, further execution is stopped and shut_down   //
// is called to clean up the application state.                                                                                          //
//                                                                                                                                       //
// The rates at which main::tick() and main::draw() are called can be adjusted at any time with the corresponding function:              //
//     - tr::sys::set_tick_frequency(240) -> tr::sys::main::tick() will be called 240 times a second                                     //
//     - tr::sys::set_draw_frequency(60) --> tr::sys::main::draw() will be called 60 times a second                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
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

	// Sets the frequency at which tick() is called (by default not active).
	void set_tick_frequency(float frequency);
	// Sets the frequency at which draw() is called (by default set to the refresh rate).
	void set_draw_frequency(float frequency);

	// User-defined functions and data (mandatory). Uncaught exceptions will display a dialog box and quit the application.
	namespace main {
		// Application metadata.
		extern const app_metadata metadata;

		// Called once at the beginning of execution, but before the initialization of any systems.
		signal parse_command_line(std::span<cstring_view> args);
		// Called once at the beginning of execution after the parsing of the command line arguments.
		signal initialize();
		// Called whenever an event needs to be handled.
		signal handle_event(event& event);
		// Main-body function used for game state ticks.
		signal tick();
		// Main-body function used for drawing and related updated.
		signal draw();
		// Called once at the end of execution.
		void shut_down();
	} // namespace main
} // namespace tr::sys