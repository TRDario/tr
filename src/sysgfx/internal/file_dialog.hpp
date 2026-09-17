/// @file
/// @brief Provides base file dialog functions.

#pragma once
#include <tr/sysgfx/dialog.hpp>

//

namespace tr::internal
{
	/// File dialog callback context.
	struct file_dialog_context
	{
		/// List of selected file paths.
		std::vector<std::filesystem::path> paths;

		/// Whether the dialog is done.
		bool done{false};
	};

	/// File dialog callback function.
	/// @param userdata Pointer to an instance of `tr::file_dialog_context`.
	/// @param files List of file paths.
	void file_dialog_callback(void* userdata, const char* const* files, int);

	/// Base open file dialog function.
	/// @param filters List of applicable filters.
	/// @param default_path Default path to start the dialog at.
	/// @param allow_multiple Whether to allow selecting multiple files.
	/// @return List of paths to the selected files.
	[[nodiscard]] std::vector<std::filesystem::path> show_open_file_dialog(std::span<const tr::dialog_filter> filters,
																		   zstring_view default_path, bool allow_multiple);

	/// Base open folder dialog function.
	/// @param default_path Default path to start the dialog at.
	/// @param allow_multiple Whether to allow selecting multiple folders.
	/// @return List of paths to the selected folders.
	[[nodiscard]] std::vector<std::filesystem::path> show_open_folder_dialog(zstring_view default_path, bool allow_multiple);
} // namespace tr::internal