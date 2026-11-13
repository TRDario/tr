/// @file
/// @brief Provides clipboard utilities.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// @brief Checks whether the clipboard is empty.
	/// @return `true` if the clipboard is empty, `false` otherwise.
	[[nodiscard]] bool clipboard_empty() noexcept;

	/// @brief Returns a copy of the clipboard text.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Copy of the clipboard text stored in a string, may be empty.
	[[nodiscard]] std::string clipboard_text();

	/// @brief Sets the clipboard text.
	/// @param text Text to set the clipboard to.
	/// @exception set_clipboard_error If setting the clipboard failed.
	void set_clipboard_text(zstring_view text);
} // namespace tr