/// @file
/// @brief Provides clipboard utilities.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// @name Clipboard
	/// @{

	/// Gets whether the clipboard is empty.
	/// @return `true` if the clipboard is empty, `false` otherwise.
	[[nodiscard]] bool clipboard_empty() noexcept;

	/// Gets the clipboard text.
	/// @return Clipboard text string, may be empty.
	[[nodiscard]] std::string clipboard_text();

	/// Sets the clipboard text.
	/// @param text Text to set the clipboard to.
	/// @exception set_clipboard_error If setting the clipboard failed.
	void set_clipboard_text(zstring_view text);

	/// @}
} // namespace tr