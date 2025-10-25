#pragma once

namespace tr::sys {
	// Message box type.
	enum class message_box_type {
		ERROR = 16,   // Error message box.
		WARNING = 32, // Warning message box.
		INFO = 64     // Information message box.
	};
	// Message box button layout.
	enum class message_box_layout {
		OK,           // Message box with a single "OK" button.
		YES_NO,       // Message box with "Yes" and "No" buttons.
		YES_NO_CANCEL // Message box with "Yes", "No" and "Cancel" buttons.
	};
	// Message box selection.
	enum class message_box_button {
		OK = 0,
		YES = 0,
		NO,
		CANCEL
	};

	// Shows a message box.
	message_box_button show_message_box(message_box_type type, message_box_layout layout, cstring_view title, cstring_view message);
	// Shows an "Fatal exception" message box.
	// In case of an out-of-memory error, it frees an emergency buffer to allow for clean-up and logging.
	void show_fatal_error_message_box(const std::exception& exception);

	// File dialog filter.
	struct dialog_filter {
		// The display name of the filter.
		cstring_view name;
		// The filter pattern.
		cstring_view pattern;
	};

	// Shows an "Open File" dialog.
	std::filesystem::path show_open_file_dialog(std::span<const dialog_filter> filters = {}, cstring_view default_path = {});
	// Shows an "Open File" dialog with the ability to choose multiple files.
	std::vector<std::filesystem::path> show_open_files_dialog(std::span<const dialog_filter> filters = {}, cstring_view default_path = {});

	// Shows an "Open Folder" dialog.
	std::filesystem::path show_open_folder_dialog(cstring_view default_path = {});
	// Shows an "Open Folder" dialog with the ability to choose multiple folders.
	std::vector<std::filesystem::path> show_open_folders_dialog(cstring_view default_path = {});

	// Shows a "Save File" dialog.
	std::filesystem::path show_save_file_dialog(std::span<const dialog_filter> filters = {}, cstring_view default_path = {});
} // namespace tr::sys