#pragma once
#include "sysgfx.hpp"
#include <imgui.h>

namespace tr {
	// Namespace containing functionality for integrating Dear ImGui with libtr.
	namespace ImGui {
		// Initializes the Dear ImGui backends needed for work with libtr.
		void Init();
		// Shuts the Dear ImGui backends needed for work with libtr down.
		void Shutdown();

		// Text input function automatically handling string resizing.
		bool InputText(const char* label, std::string& str, ImGuiInputTextFlags flags = 0);

		// Gets the Dear ImGui texture ID for a libtr texture.
		u64 GetTextureID(const gfx::texture& texture);

		// Processes an event for Dear ImGui.
		void ProcessEvent(const sys::event& event);

		// Prepares the Dear ImGui backends needed for work with libtr for a new frame.
		void NewFrame();
		// Draws a Dear ImGui frame.
		void Draw();
	} // namespace ImGui
} // namespace tr