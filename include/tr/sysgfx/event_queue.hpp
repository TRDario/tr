#pragma once

namespace tr {
	class timer;
}

namespace tr::sys {
	// Enables sending text input events.
	void enable_text_input_events();
	// Disables sending text input events.
	void disable_text_input_events();
} // namespace tr::sys