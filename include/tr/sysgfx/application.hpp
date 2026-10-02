/// @file
/// @brief Provides `tr::application` and `tr::run_main_loop()`.
/// @details
/// In order to make use of most system and graphics module functionality, the tr main loop must be invoked in `main()`. The main loop
/// function `tr::run_main_loop()` takes an object representing the application state. The application object contains the virtual callback
/// functions `tr::application::initialize()`, `tr::application::handle_event()`, `tr::application::update()`, and
/// `tr::application::shut_down()`. These functions are invoked when appropriate by the main loop.
///
/// The lifetime of the system and graphics module functionality is limited to the duration of the main loop. Do not attempt to call system
/// and graphics functions outside of the loop, outside of some explicit exceptions.

#pragma once
#include <tr/sysgfx/event.hpp>
#include <tr/sysgfx/internal/application.hpp>
#include <tr/utility/chrono.hpp>

//

namespace tr
{
	/// Application metadata.
	struct application_metadata
	{
		/// Supported application types.
		enum class application_type
		{
			/// The application is a game.
			game,

			/// The application type is unspecified.
			application
		};

		//

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

		/// Application type.
		application_type type{application_type::application};
	};

	/// Interface for an application run in the main loop of tr.
	class application
	{
	  public:
		/// Signals used for defining the control flow of the application callbacks.
		enum class signal
		{
			/// Continue execution.
			proceed,

			/// Regular exit.
			exit,

			/// Abnormal exit.
			abort
		};

		/// @name Constructors and destructors
		/// @{

		/// Constructs an application.
		[[nodiscard]] application() noexcept = default;

		/// Applications are not copyable.
		application(const application&) = delete;

		/// Applications are not movable.
		application(application&&) = delete;

		/// Destroys the application.
		/// @details Uncaught exceptions from this function will display a dialog box.
		virtual ~application() noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Applications are not copyable.
		application& operator=(const application&) = delete;

		/// Applications are not movable.
		application& operator=(application&&) = delete;

		/// @}

		/// Callback function called once at the beginning of execution after the initialization of systems.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param args Application command-line arguments.
		/// @return Application state signal after initialization.
		virtual signal initialize(std::span<const zstring_view> args);

		/// Callback function called whenever an event needs to be handled. Not guaranteed to be called on the main thread.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param event Event to handle.
		/// @return Application state signal after handling the event.
		virtual signal handle_event(const event& event);

		/// Primary update callback function.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param delta Time since the last update.
		/// @return Application state signal after updating.
		virtual signal update(duration delta);
	};

	/// Sentinel representing an uncapped update frequency (the default behavior).
	inline constexpr float uncapped_update_frequency{0};

	/// @name Application
	/// @{

	/// Runs an application.
	/// @tparam Application Application type fulfilling the `tr::application` interface.
	/// @tparam Args Types of the arguments to the application constructor.
	/// @param argc, argv Command-line arguments.
	/// @param metadata Metadata of the application.
	/// @param args Arguments to the application constructor.
	/// @return Exit code of the application.
	template <typename Application, typename... Args>
		requires std::derived_from<Application, application> && std::constructible_from<Application, Args...>
	[[nodiscard]] int run_application(int argc, const char** argv, const application_metadata& metadata, Args&&... args)
	{
		internal::set_application_metadata(metadata);
		std::unique_ptr<Application> application{std::make_unique<Application>(std::forward<Args>(args)...)};
		return internal::run_application(std::move(application), argc, argv);
	}

	/// Sets the frequency at which update() is called in the main loop (by default uncapped).
	/// @param frequency Frequency at which update() should be called in the main loop, or `tr::uncapped_update_frequency`.
	void set_update_frequency(float frequency) noexcept;

	/// @}
} // namespace tr