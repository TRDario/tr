/// @file
/// @brief Provides `tr::application` and `tr::run_main_loop()`.
/// @details
/// In order to make use of most system and graphics module functionality, the tr application loop must be invoked in `main()`. The
/// application loop function `tr::run_application()` initializes internal subsystems and constructs an application state object from the
/// arguments passed into the function. The application object contains the virtual callback functions `tr::application::initialize()`,
/// `tr::application::handle_event()`, and `tr::application::update()`. These functions are invoked when appropriate by the application
/// loop.
///
/// The lifetime of the system and graphics module functionality is limited to the duration of the application loop. Do not attempt to call
/// system and graphics functions outside of the loop, outside of some explicit exceptions.

#pragma once
#include <tr/sysgfx/event_view.hpp>
#include <tr/sysgfx/internal/application.hpp>
#include <tr/utility/chrono.hpp>
#include <tr/utility/zstring_view_zspan.hpp>

//

namespace tr
{
	/// @brief Application metadata.
	struct application_metadata
	{
		/// @brief Supported application types.
		enum class type
		{
			/// @brief The application is a game.
			game,

			/// @brief The application type is unspecified.
			application
		};

		//

		/// @brief Name of the application.
		zstring_view name{};

		/// @brief Version of the application.
		zstring_view version{};

		/// @brief Identifier of the application.
		zstring_view identifier{};

		/// @brief Developer of the application.
		zstring_view developer{};

		/// @brief Short copyright notice.
		zstring_view copyright{};

		/// @brief URL relevant to the application.
		zstring_view url{};

		/// @brief Application type.
		type type{type::application};
	};

	/// @brief Interface for an application run in the main loop of tr.
	class application
	{
	  public:
		/// @brief Signals used for defining the control flow of the application callbacks.
		enum class signal
		{
			/// @brief Continue execution.
			proceed,

			/// @brief Regular exit.
			exit,

			/// @brief Abnormal exit.
			abort
		};

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an application.
		[[nodiscard]] application() noexcept = default;

		/// @brief Applications are not copyable.
		application(const application&) = delete;

		/// Applications are not copyable.
		application& operator=(const application&) = delete;

		/// @brief Applications are not movable.
		application(application&&) = delete;

		/// Applications are not movable.
		application& operator=(application&&) = delete;

		/// @brief Destroys the application.
		virtual ~application() noexcept = default;

		/// @}
		/// @name Callbacks
		/// @{

		/// @brief Callback function called once at the beginning of execution after the initialization of systems.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param args Application command-line arguments.
		/// @return Application state signal after initialization.
		virtual signal initialize(zstring_view_zspan args);

		/// @brief Callback function called whenever an event needs to be handled. Not guaranteed to be called on the main thread.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param event Event to handle.
		/// @return Application state signal after handling the event.
		virtual signal handle_event(event_view event);

		/// @brief Primary update callback function.
		/// @details Uncaught exceptions from this function will display a dialog box and quit the application.
		/// @param delta Time since the last update.
		/// @return Application state signal after updating.
		virtual signal update(duration delta);

		/// @}
	};

	/// @brief Sentinel representing an uncapped update frequency (the default behavior).
	inline constexpr float uncapped_update_frequency{0};

	//

	/// @brief Runs an application.
	/// @tparam Application Application type fulfilling the `tr::application` interface.
	/// @tparam Args Types of the arguments to the application constructor.
	/// @param argc, argv Command-line arguments.
	/// @param metadata Metadata of the application.
	/// @param args Arguments to the application constructor.
	/// @return Exit code of the application.
	template <typename Application, typename... Args>
		requires std::derived_from<Application, application> && std::constructible_from<Application, Args...>
	[[nodiscard]] int run_application(int argc, char** argv, const application_metadata& metadata, Args&&... args)
	{
		internal::set_application_metadata(metadata);
		std::unique_ptr<Application> application{std::make_unique<Application>(std::forward<Args>(args)...)};
		return internal::run_application(std::move(application), argc, argv);
	}

	/// @brief Sets the frequency at which update() is called in the main loop (by default uncapped).
	/// @param frequency Frequency at which update() should be called in the main loop, or `tr::uncapped_update_frequency`.
	void set_update_frequency(float frequency) noexcept;
} // namespace tr