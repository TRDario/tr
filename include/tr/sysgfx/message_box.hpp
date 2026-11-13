/// @file
/// @brief Provides `tr::show_message_box()` and related datatypes.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// @brief Message box type.
	enum class message_box_type : unsigned int
	{
		/// @brief Error message box.
		/// @hideinitializer
		error = 16,

		/// @brief Warning message box.
		/// @hideinitializer
		warning = 32,

		/// @brief Information message box.
		/// @hideinitializer
		info = 64
	};

	/// @brief Message box button layout.
	enum class message_box_layout
	{
		/// @brief Message box with a single "OK" button.
		ok,

		/// @brief Message box with "Yes" and "No" buttons.
		yes_no,

		/// @brief Message box with "Yes", "No" and "Cancel" buttons.
		yes_no_cancel
	};

	/// @brief Message box selection.
	enum class message_box_button
	{
		/// @brief User pressed "OK" or dismissed a `message_box_layout::ok` message box.
		/// @hideinitializer
		ok = 0,

		/// @brief User pressed "Yes".
		/// @hideinitializer
		yes = 0,

		/// @brief User pressed "No" or dismissed a `message_box_layout::yes_no` message box.
		no,

		/// @brief User pressed "Cancel" or dismissed a `message_box_layout::yes_no_cancel` message box.
		cancel
	};

	//

	/// @brief Shows a message box.
	/// @param type Message box type.
	/// @param layout Message box layout.
	/// @param title Message box title.
	/// @param message Message box message.
	/// @return Button the user pressed.
	message_box_button show_message_box(message_box_type type, message_box_layout layout, zstring_view title,
										zstring_view message) noexcept;
} // namespace tr