/// @file
/// @brief Provides clipboard utilities.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"

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
		set_clipboard_error();

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Clipboard setting error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string_view m_description;
	};

	/// @name Clipboard
	/// @{

	/// Gets whether the clipboard is empty.
	/// @return `true` if the clipboard is empty, `false` otherwise.
	bool clipboard_empty();

	/// Gets the clipboard text.
	/// @return Clipboard text string, may be empty.
	std::string clipboard_text();

	/// Sets the clipboard text.
	/// @param text Text to set the clipboard to.
	/// @exception set_clipboard_error If setting the clipboard failed.
	void set_clipboard_text(zstring_view text);

	/// @}
} // namespace tr