///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an interface for triggering native OS dialog boxes.                                                                          //
//                                                                                                                                       //
// Message boxes can be shown with tr::sys::show_message_box. There are three classes of message box (error, warning, info), as well as  //
// three button layouts (ok, yes/no, yes/no/cancel). The function blocks until a button is pressed, at which point the value of the      //
// pressed button is returned. tr::sys::show_fatal_error_message_box shows a "Fatal exception" message box given an exception:           //
//     - tr::sys::show_message_box(tr::sys::message_box_type::error, tr::sys::message_box_layout::ok, "Foo", "Bar")                      //
//       -> displays an error message box with an "OK" button with the title "Foo" and message "Bar", returns message_box_button::OK     //
//     - tr::sys::show_message_box(tr::sys::message_box_type::info, tr::sys::message_box_layout::yes_no_cancel, "Foo", "Save?")          //
//       -> displays an info message box with the buttons "Yes", "No", and "Cancel" with the title "Foo" and message "Save?",            //
//          may return message_box_button::yes, message_box_button::no, or message_box_button::cancel                                    //
//     - tr::sys::show_fatal_error_message_box(tr::custom_exception{"Example error", "Reason goes here.", "Details go here."})           //
//       -> displays an error message box with an "Ok" button with the title "[APP NAME] - Fatal Error" and message:                     //
//            A fatal error has occured (Example error).                                                                                 //
//            Reason goes here.                                                                                                          //
//            Details go here.                                                                                                           //
//            Press OK to exit the application.                                                                                          //
//                                                                                                                                       //
// File dialogs (for selecting one or multiple files, or a folder) can be shown with their respective functions. All of the functions    //
// take a span of dialog filters (name-pattern pair as such: {"Image files", "png;jpg;bmp"}, {"Text files", "txt"}, {"All files", "*"})  //
// and an optional default path, and return the path(s) of the selected file(s)/folder(s), or an empty path/list if nothing was selected://
//     - tr::sys::show_open_file_dialog(filters)                                                                                         //
//       -> shows a dialog that allows selecting one file with the passed filters, returns a path to the selected file or an empty path  //
//          if nothing was selected                                                                                                      //
//     - tr::sys::show_open_file_dialog(filters, "~/Example")                                                                            //
//       -> shows a dialog that allows selecting one file with the passed filters, starting from the "~/Example" folder,                 //
//          returns a path to the selected file or an empty path if nothing was selected                                                 //
//     - tr::sys::show_open_files_dialog(filters)                                                                                        //
//       -> shows a dialog that allows selecting one or multiple files with the passed filters, returns a list of paths to the selected  //
//          files (list may be empty)                                                                                                    //
//     - tr::sys::show_open_folder_dialog(filters)                                                                                       //
//       -> shows a dialog that allows selecting one folder with the passed filters, returns a path to the selected folder or an empty   //
//          path if nothing was selected                                                                                                 //
//     - tr::sys::show_open_folders_dialog(filters)                                                                                      //
//       -> shows a dialog that allows selecting one or multiple folders with the passed filters, returns a list of paths to the selected//
//          folders (list may be empty)                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
	// Message box type.
	enum class message_box_type {
		error = 16,   // Error message box.
		warning = 32, // Warning message box.
		info = 64     // Information message box.
	};
	// Message box button layout.
	enum class message_box_layout {
		ok,           // Message box with a single "OK" button.
		yes_no,       // Message box with "Yes" and "No" buttons.
		yes_no_cancel // Message box with "Yes", "No" and "Cancel" buttons.
	};
	// Message box selection.
	enum class message_box_button {
		ok = 0,
		yes = 0,
		no,
		cancel
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