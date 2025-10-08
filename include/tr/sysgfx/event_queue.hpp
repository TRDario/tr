#pragma once
#include "display.hpp"

namespace tr {
	class timer;
}

namespace tr::sys {
	// Creates a timer that sends tick events.
	timer create_tick_timer(float frequency, int id = 0);
	// Creates a timer that sends draw events.
	timer create_draw_timer(float frequency = refresh_rate());

	// Enables sending text input events.
	void enable_text_input_events();
	// Disables sending text input events.
	void disable_text_input_events();
} // namespace tr::sys