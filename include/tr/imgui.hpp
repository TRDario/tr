/// @file
/// @brief Provides functionality for integrating tr with Dear ImGui.
/// @details
/// Using this module requires the option TR_BUILD_IMGUI to be turned on in CMake. TR_BUILD_IMGUI, in turn, depends on TR_BUILD_SYSGFX.

#pragma once
#include <imgui.h>                 // IWYU pragma: export
#include <misc/cpp/imgui_stdlib.h> // IWYU pragma: export

namespace tr
{
	class event;
	class graphics_context;
	class texture;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Namespace containing functionality for integrating Dear ImGui with tr.
	namespace ImGui
	{
		/// @name Management
		/// @{

		/// Initializes the Dear ImGui backends needed to work with tr.
		/// @note This function should be called once after `::ImGui::CreateContext`.
		/// @param context Graphics context the ImGUI backend will be associated with.
		void Init(graphics_context& context);

		/// Processes an event for Dear ImGui.
		/// @note This function should preferably be called on every `tr_app::handle_event` call.
		/// @param event Event to process.
		void ProcessEvent(const event& event);

		/// Shuts the Dear ImGui backends needed to work with tr down.
		/// @note This function should be called once before `::ImGui::DestroyContext`.
		void Shutdown();

		/// @}
		/// @name Drawing
		/// @{

		/// Gets the Dear ImGui texture ID for a tr texture view.
		/// @param texture Texture to get the ImGui ID of.
		/// @return ID associated with the texture that may be passed to ImGui functions.
		ImTextureID GetTextureID(texture_view texture);

		/// Prepares the Dear ImGui backends needed to work with tr for a new frame.
		/// @note This function should be called after `::ImGui::NewFrame`.
		void NewFrame();

		/// Draws a Dear ImGui frame.
		/// @note This function should be called after `::ImGui::Render`.
		/// @param context Graphics context to draw the frame on.
		void Draw(graphics_context& context);

		/// @}
	} // namespace ImGui
} // namespace tr