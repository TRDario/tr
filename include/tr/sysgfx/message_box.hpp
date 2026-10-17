/// @file
/// @brief Provides `tr::show_message_box()` and related datatypes.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// Message box type.
	enum class message_box_type : unsigned int
	{
		/// Error message box.
		error = 16,

		/// Warning message box.
		warning = 32,

		/// Information message box.
		info = 64
	};

	/// Message box button layout.
	enum class message_box_layout
	{
		/// Message box with a single "OK" button.
		ok,

		/// Message box with "Yes" and "No" buttons.
		yes_no,

		/// Message box with "Yes", "No" and "Cancel" buttons.
		yes_no_cancel
	};

	/// Message box selection.
	enum class message_box_button
	{
		/// User pressed "OK" or dismissed a `message_box_layout::ok` message box.
		ok = 0,

		/// User pressed "Yes".
		yes = 0,

		/// User pressed "No" or dismissed a `message_box_layout::yes_no` message box.
		no,

		/// User pressed "Cancel" or dismissed a `message_box_layout::yes_no_cancel` message box.
		cancel
	};

	/// @name Message box
	/// @{

	/// Shows a message box.
	/// @param type Message box type.
	/// @param layout Message box layout.
	/// @param title Message box title.
	/// @param message Message box message.
	/// @return Button the user pressed.
	message_box_button show_message_box(message_box_type type, message_box_layout layout, zstring_view title,
										zstring_view message) noexcept;

	/// @}
} // namespace tr