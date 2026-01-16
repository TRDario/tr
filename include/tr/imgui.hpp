///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functionality for integrating tr with Dear ImGui.                                                                            //
//                                                                                                                                       //
// Using this module requires the option TR_BUILD_IMGUI to be turned on in CMake. TR_BUILD_IMGUI, in turn, depends on TR_BUILD_SYSGFX.   //
//                                                                                                                                       //
// Nothing from this module is allowed to be called before a window is opened.                                                           //
//                                                                                                                                       //
// tr::ImGui::Init should be called after ImGui::CreateContext.                                                                          //
//                                                                                                                                       //
// tr::ImGui::ProcessEvent should preferably be called on every main::handle_event call.                                                 //
//                                                                                                                                       //
// tr::ImGui::NewFrame should be called before ImGui::NewFrame.                                                                          //
//                                                                                                                                       //
// Use tr::ImGui::GetTextureID to get a Dear ImGui-compatible ID for a tr texture.                                                       //
//     - ImGui::Image(tr::ImGui::GetTextureID(texture), std::bit_cast<ImVec2>(texture.size()))                                           //
//                                                                                                                                       //
// tr::ImGui::Draw should be called after ImGui::Render.                                                                                 //
//                                                                                                                                       //
// tr::ImGui::Shutdown should be called before ImGui::DestroyContext.                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sysgfx.hpp"
#include <imgui.h>                 // IWYU pragma: export
#include <misc/cpp/imgui_stdlib.h> // IWYU pragma: export

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Namespace containing functionality for integrating Dear ImGui with tr.
	namespace ImGui {
		// Initializes the Dear ImGui backends needed for work with tr.
		void Init();
		// Shuts the Dear ImGui backends needed for work with tr down.
		void Shutdown();

		// Gets the Dear ImGui texture ID for a tr texture.
		ImTextureID GetTextureID(const gfx::texture& texture);

		// Processes an event for Dear ImGui.
		void ProcessEvent(const sys::event& event);

		// Prepares the Dear ImGui backends needed for work with tr for a new frame.
		void NewFrame();
		// Draws a Dear ImGui frame.
		void Draw();
	} // namespace ImGui
} // namespace tr