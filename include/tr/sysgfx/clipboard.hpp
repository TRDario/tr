/// @file
/// @brief Provides clipboard utilities.

#pragma once
#include <tr/utility/exception.hpp>
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// Clipboard setting error.
	class set_clipboard_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a clipboard setting error.
		[[nodiscard]] set_clipboard_error() noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Clipboard setting error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

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