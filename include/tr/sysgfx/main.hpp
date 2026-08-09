/// @file
/// @brief Provides functionality related to the main loop of the program.
/// @details
/// Unlike a standard C++ program, programs using tr do not have to define main(). Instead, a number of functions in the namespace `tr_app`
/// have to be defined, as well as tr_app::metadata, a struct containing basic application metadata.
///
/// Most functions in app, with the exception of shut_down, return a signal. If `tr::signal::proceed` is returned, execution will continue
/// as normal. If `tr::signal::exit` or `tr::signal::abort` is returned, further execution is stopped and shut_down is called to clean up
/// the application state.

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/exception.hpp"
#include "event.hpp"

//

namespace tr
{
	/// Error thrown when system initialization fails.
	class init_error : public tr::exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param description Description of the error.
		init_error(std::string_view description);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Initialization error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

	//

	/// Signals used for controlling control flow of the user defined callbacks.
	/// @details
	/// Most functions in app, with the exception of shut_down, return a signal. If `tr::signal::proceed` is returned, execution will
	/// continue as normal. If `tr::signal::exit` or `tr::signal::abort` is returned, further execution is stopped and shut_down called to
	/// clean up the application state.
	enum class signal
	{
		/// Continue execution.
		proceed,
		/// Regular exit.
		exit,
		/// Abnormal exit.
		abort
	};

	/// Supported application types.
	enum class app_type
	{
		/// The application is a game.
		game,
		/// The application type is unspecified.
		application
	};

	/// Application metadata.
	struct app_metadata
	{
		/// Name of the application.
		zstring_view name{};

		/// Version of the application.
		zstring_view version{};

		/// Identifier of the application.
		zstring_view identifier{};

		/// Developer of the application.
		zstring_view developer{};

		/// Short copyright notice.
		zstring_view copyright{};

		/// URL relevant to the application.
		zstring_view url{};

		/// Type of the application.
		app_type type{app_type::application};
	};

	//

	/// Sentinel representing an uncapped update frequency (the default behavior).
	inline constexpr float uncapped_update_frequency{0};

	/// @name Update frequency
	/// @{

	/// Sets the frequency at which update() is called (by default uncapped).
	/// @param frequency Frequency at which update() should be called, or uncapped_update_frequency.
	void set_update_frequency(float frequency);

	/// @}
} // namespace tr

/// User-defined functions and data (mandatory). Uncaught exceptions will display a dialog box and quit the application.
namespace tr_app
{
	/// Application metadata.
	extern const tr::app_metadata metadata;

	/// Called once at the beginning of execution, but before the initialization of any systems.
	/// @param args Command-line argument span.
	/// @return Program state signal after command-line parsing.
	tr::signal parse_command_line(std::span<tr::zstring_view> args);

	/// Called once at the beginning of execution after the parsing of the command line arguments.
	/// @return Program state signal after initialization.
	tr::signal initialize();

	/// Called whenever an event needs to be handled. Not guaranteed to be called on the main thread.
	/// @param event Event to handle.
	/// @return Program state signal after handling the event.
	tr::signal handle_event(const tr::event& event);

	/// Primary update function.
	/// @param delta Time since the last update.
	/// @return Program state signal after updating.
	tr::signal update(tr::duration delta);

	/// Called once at the end of execution.
	void shut_down();
} // namespace tr_app