/// @file
/// @brief Provides an interface for triggering native OS dialog boxes.

#pragma once
#include "../utility/zstring_view.hpp"

//

namespace tr
{
	/// Message box type.
	enum class message_box_type : unsigned int
	{
		error = 16,   // Error message box.
		warning = 32, // Warning message box.
		info = 64     // Information message box.
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

	/// File dialog filter.
	struct dialog_filter
	{
		// Display name of the filter.
		zstring_view name;

		// Filter pattern.
		zstring_view pattern;
	};

	/// @name Dialogs
	/// @{

	/// Shows a message box.
	/// @param type Message box type.
	/// @param layout Message box layout.
	/// @param title Message box title.
	/// @param message Message box message.
	/// @return Button the user pressed.
	message_box_button show_message_box(message_box_type type, message_box_layout layout, zstring_view title, zstring_view message);

	/// Shows an "Open File" dialog.
	/// @param filters List of applicable file filters.
	/// @param default_path Default path to start the dialog at.
	/// @return Path to the selected file (or an empty path if not selected).
	std::filesystem::path show_open_file_dialog(std::span<const dialog_filter> filters = {}, zstring_view default_path = {});

	/// Shows an "Open File" dialog with the ability to choose multiple files.
	/// @param filters List of applicable file filters.
	/// @param default_path Default path to start the dialog at.
	/// @return List of paths to the selected files.
	std::vector<std::filesystem::path> show_open_files_dialog(std::span<const dialog_filter> filters = {}, zstring_view default_path = {});

	/// Shows an "Open Folder" dialog.
	/// @param default_path Default path to start the dialog at.
	/// @return Path to the selected folder (or an empty path if not selected).
	std::filesystem::path show_open_folder_dialog(zstring_view default_path = {});

	// Shows an "Open Folder" dialog with the ability to choose multiple folders.
	/// @param default_path Default path to start the dialog at.
	/// @return List of paths to the selected folders.
	std::vector<std::filesystem::path> show_open_folders_dialog(zstring_view default_path = {});

	// Shows a "Save File" dialog.
	/// @param filters List of applicable file filters.
	/// @param default_path Default path to start the dialog at.
	/// @return Path to the selected file (or an empty path if not selected).
	std::filesystem::path show_save_file_dialog(std::span<const dialog_filter> filters = {}, zstring_view default_path = {});

	/// @}
} // namespace tr