#pragma once
#include "event_types.hpp"

namespace tr::sys {
	// Return codes used for controlling control flow of the user defined callbacks.
	enum class return_code {
		// Continue execution.
		CONTINUE,
		// Regular exit.
		EXIT,
		// Abnormal exit.
		ABORT
	};

	// Sets various app information.
	void set_app_information(const char* developer, const char* name, const char* version);
	// Sets the frequency at which tr::sys::user_defined::iterate() is called.
	void set_iteration_frequency(float frequency);

	// User-defined functions (mandatory).
	namespace user_defined {
		// Called once at the beginning of execution.
		return_code initialize(std::span<const char*> args) noexcept;
		// Called whenever an event needs to be handled.
		return_code handle_event(event& event) noexcept;
		// Main-body function to be used for rendering and the like.
		return_code iterate() noexcept;
		// Called once at the end of execution.
		void quit() noexcept;
	} // namespace user_defined
} // namespace tr::sys