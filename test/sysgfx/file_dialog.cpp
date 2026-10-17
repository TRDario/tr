/// @file
/// @brief Tests sysgfx/file_dialog.hpp.
/// @details This test only checks whether the dialogs compile properly, it does not test the behavior at runtime.

#include <tr/sysgfx/file_dialog.hpp>

//

/// Placeholder function taking a single file path.
void single_file_function(tr::zstring_view, int) {}

/// Placeholder function taking a span of file paths.
void multiple_files_function(tr::zstring_view_zspan, int) {}

/// Placeholder function taking a single folder path.
void single_folder_function(tr::zstring_view) {}

/// Placeholder function taking a span of folder paths.
void folders_function(tr::zstring_view_zspan) {}

//

/// Unused function used to test compilation of file dialog functions.
[[maybe_unused]] void file_dialog_compilation_test()
{
	tr::show_open_file_dialog(single_file_function);
	tr::show_open_file_dialog([](tr::zstring_view, int) {});
	tr::show_open_file_dialog([state = std::make_unique<int>(0)](tr::zstring_view, int) { (void)state; });
	tr::show_open_files_dialog(multiple_files_function);
	tr::show_open_files_dialog([](tr::zstring_view_zspan, int) {});
	tr::show_open_files_dialog([state = std::make_unique<int>(0)](tr::zstring_view_zspan, int) { (void)state; });
	tr::show_save_file_dialog(single_file_function);
	tr::show_save_file_dialog([](tr::zstring_view, int) {});
	tr::show_save_file_dialog([state = std::make_unique<int>(0)](tr::zstring_view, int) { (void)state; });
	tr::show_open_folder_dialog(single_folder_function);
	tr::show_open_folder_dialog([](tr::zstring_view) {});
	tr::show_open_folder_dialog([state = std::make_unique<int>(0)](tr::zstring_view) { (void)state; });
	tr::show_open_folders_dialog(folders_function);
	tr::show_open_folders_dialog([](tr::zstring_view_zspan) {});
	tr::show_open_folders_dialog([state = std::make_unique<int>(0)](tr::zstring_view_zspan) { (void)state; });
}