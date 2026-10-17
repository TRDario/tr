/// @file
/// @brief Provides file dialog internals.

#pragma once
#include <tr/utility/zstring_view_zspan.hpp>

namespace tr
{
	struct file_dialog_filter;
}

//

namespace tr::internal
{
	/// Checks whether a callable is statelessly invocable.
	/// @tparam Callable Callable to check for stateless invocability.
	/// @tparam FunctionPointer Function pointer the callable should be equivalent to.
	template <typename Callable, typename FunctionPointer>
	inline constexpr bool is_stateless_invocable_v{std::convertible_to<Callable, FunctionPointer> ||
												   requires { static_cast<FunctionPointer>(&std::remove_cvref_t<Callable>::operator()); }};

	//

	/// Raw file dialog callback function pointer.
	using raw_file_dialog_callback = void (*)(void* userdata, const char* const* paths, int filter_index);

	/// @name File dialogs
	/// @{

	/// Shows a non-blocking "Open File" dialog.
	/// @param callback Callback function pointer.
	/// @param userdata Userdata passed to the callback.
	/// @param filters List of applicable file filters.
	/// @param default_path Default path to start the dialog at.
	/// @param allow_multiple Whether selecting multiple files is allowed.
	void show_open_file_dialog(raw_file_dialog_callback callback, void* userdata, std::span<const file_dialog_filter> filters,
							   zstring_view default_path, bool allow_multiple) noexcept;

	/// Shows a non-blocking "Save File" dialog.
	/// @param callback Callback function pointer.
	/// @param userdata Userdata passed to the callback.
	/// @param filters List of applicable file filters.
	/// @param default_path Default path to start the dialog at.
	void show_save_file_dialog(raw_file_dialog_callback callback, void* userdata, std::span<const file_dialog_filter> filters,
							   zstring_view default_path) noexcept;

	/// Shows a non-blocking "Open Folder" dialog.
	/// @param callback Callback function pointer.
	/// @param userdata Userdata passed to the callback.
	/// @param default_path Default path to start the dialog at.
	/// @param allow_multiple Whether selecting multiple files is allowed.
	void show_open_folder_dialog(raw_file_dialog_callback callback, void* userdata, zstring_view default_path,
								 bool allow_multiple) noexcept;

	/// @}
} // namespace tr::internal