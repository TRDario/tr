#pragma once
#include <tr/sysgfx.hpp>

namespace tr {
	// Namespace containing functionality for integrating Dear ImGui with libtr.
	namespace imgui {
		// Initializes the Dear ImGui backends needed for work with libtr.
		void initialize();
		// Shuts the Dear ImGui backends needed for work with libtr down.
		void shut_down();

		// Gets the Dear ImGui texture ID for a libtr texture.
		u64 get_texture_id(gfx::texture_ref texture);

		// Processes an event for Dear ImGui.
		void process_event(const sys::event& event);

		// Prepares the Dear ImGui backends needed for work with libtr for a new frame.
		void new_frame();
		// Draws a Dear ImGui frame.
		void draw();
	} // namespace imgui
} // namespace tr