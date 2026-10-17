/// @file
/// @brief Provides file dialog functions.

#pragma once
#include <tr/sysgfx/internal/file_dialog.hpp>

//

namespace tr
{
	/// File dialog filter.
	struct file_dialog_filter
	{
		/// Display name of the filter.
		zstring_view name;

		/// Filter pattern.
		zstring_view pattern;
	};

	/// @name File dialogs
	/// @{

	/// Shows an "Open File" dialog and asynchronously invokes a callback after it is closed.
	/// @details The callback may be called on a separate thread. It is not recommended to throw exceptions from it.
	/// @tparam Callback Type of the callback callable.
	/// @param callback Callback callable.
	/// @param filters List of applicable file filters. The list and all of the string views inside the filters contained within it must
	/// remain valid until the callback is called.
	/// @param default_path Default path to start the dialog at.
	template <std::invocable<zstring_view, int> Callback>
	void show_open_file_dialog(Callback&& callback, std::span<const file_dialog_filter> filters = {}, zstring_view default_path = {})
		noexcept(internal::is_stateless_invocable_v<Callback, void (*)(zstring_view, int)>)
	{
		using function_pointer = void (*)(zstring_view, int);
		if constexpr (std::convertible_to<Callback, function_pointer>) {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(std::forward<Callback>(callback))),
				filters,
				default_path,
				false
			);
			// clang-format on
		}
		else if constexpr (requires { static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator()); }) {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator())),
				filters,
				default_path,
				false
			);
			// clang-format on
		}
		else {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					(*static_cast<std::remove_cvref_t<Callback>*>(callback))(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
					delete static_cast<std::remove_cvref_t<Callback>*>(callback);
				},
				new std::remove_cvref_t<Callback>{std::forward<Callback>(callback)},
				filters,
				default_path,
				false
			);
			// clang-format on
		}
	}

	/// Shows an "Open File" dialog with the ability to choose multiple files and asynchronously invokes a callback after it is closed.
	/// @details The callback may be called on a separate thread. It is not recommended to throw exceptions from it.
	/// @tparam Callback Type of the callback callable.
	/// @param callback Callback callable.
	/// @param filters List of applicable file filters. The list and all of the string views inside the filters contained within it must
	/// remain valid until the callback is called.
	/// @param default_path Default path to start the dialog at.
	template <std::invocable<zstring_view_zspan, int> Callback>
	void show_open_files_dialog(Callback&& callback, std::span<const file_dialog_filter> filters = {}, zstring_view default_path = {})
		noexcept(internal::is_stateless_invocable_v<Callback, void (*)(zstring_view_zspan, int)>)
	{
		using function_pointer = void (*)(zstring_view_zspan, int);
		if constexpr (std::convertible_to<Callback, function_pointer>) {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(zstring_view_zspan{paths}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(callback)),
				filters,
				default_path,
				true
			);
			// clang-format on
		}
		else if constexpr (requires { static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator()); }) {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(zstring_view_zspan{paths}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator())),
				filters,
				default_path,
				true
			);
			// clang-format on
		}
		else {
			// clang-format off
			internal::show_open_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					(*static_cast<std::remove_cvref_t<Callback>*>(callback))(zstring_view_zspan{paths}, filter_index);
					delete static_cast<std::remove_cvref_t<Callback>*>(callback);
				},
				new std::remove_cvref_t<Callback>{std::forward<Callback>(callback)},
				filters,
				default_path,
				true
			);
			// clang-format on
		}
	}

	/// Shows a "Save File" dialog and asynchronously invokes a callback after it is closed.
	/// @details The callback may be called on a separate thread. It is not recommended to throw exceptions from it.
	/// @tparam Callback Type of the callback callable.
	/// @param callback Callback callable.
	/// @param filters List of applicable file filters. The list and all of the string views inside the filters contained within it must
	/// remain valid until the callback is called.
	/// @param default_path Default path to start the dialog at.
	template <std::invocable<zstring_view, int> Callback>
	void show_save_file_dialog(Callback&& callback, std::span<const file_dialog_filter> filters = {}, zstring_view default_path = {})
		noexcept(internal::is_stateless_invocable_v<Callback, void (*)(zstring_view, int)>)
	{
		using function_pointer = void (*)(zstring_view, int);
		if constexpr (std::convertible_to<Callback, function_pointer>) {
			// clang-format off
			internal::show_save_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(callback)),
				filters,
				default_path
			);
			// clang-format on
		}
		else if constexpr (requires { static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator()); }) {
			// clang-format off
			internal::show_save_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator())),
				filters,
				default_path
			);
			// clang-format on
		}
		else {
			// clang-format off
			internal::show_save_file_dialog(
				[](void* callback, const char* const* paths, int filter_index) {
					(*static_cast<std::remove_cvref_t<Callback>*>(callback))(paths != nullptr ? zstring_view{paths[0]} : zstring_view{}, filter_index);
					delete static_cast<std::remove_cvref_t<Callback>*>(callback);
				},
				new std::remove_cvref_t<Callback>{std::forward<Callback>(callback)},
				filters,
				default_path
			);
			// clang-format on
		}
	}

	/// Shows an "Open Folder" dialog and asynchronously invokes a callback after it is closed.
	/// @details The callback may be called on a separate thread. It is not recommended to throw exceptions from it.
	/// @tparam Callback Type of the callback callable.
	/// @param callback Callback callable.
	/// @param default_path Default path to start the dialog at.
	template <std::invocable<zstring_view> Callback>
	void show_open_folder_dialog(Callback&& callback, zstring_view default_path = {})
		noexcept(internal::is_stateless_invocable_v<Callback, void (*)(zstring_view)>)
	{
		using function_pointer = void (*)(zstring_view);
		if constexpr (std::convertible_to<Callback, function_pointer>) {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{});
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(callback)),
				default_path,
				false
			);
			// clang-format on
		}
		else if constexpr (requires { static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator()); }) {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					reinterpret_cast<function_pointer>(callback)(paths != nullptr ? zstring_view{paths[0]} : zstring_view{});
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator())),
				default_path,
				false
			);
			// clang-format on
		}
		else {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					(*static_cast<std::remove_cvref_t<Callback>*>(callback))(paths != nullptr ? zstring_view{paths[0]} : zstring_view{});
					delete static_cast<std::remove_cvref_t<Callback>*>(callback);
				},
				new std::remove_cvref_t<Callback>{std::forward<Callback>(callback)},
				default_path,
				false
			);
			// clang-format on
		}
	}

	/// Shows an "Open Folder" dialog with the ability to choose multiple files and asynchronously invokes a callback after it is closed.
	/// @details The callback may be called on a separate thread. It is not recommended to throw exceptions from it.
	/// @tparam Callback Type of the callback callable.
	/// @param callback Callback callable.
	/// @param default_path Default path to start the dialog at.
	template <std::invocable<zstring_view_zspan> Callback>
	void show_open_folders_dialog(Callback&& callback, zstring_view default_path = {})
		noexcept(internal::is_stateless_invocable_v<Callback, void (*)(zstring_view_zspan)>)
	{
		using function_pointer = void (*)(zstring_view_zspan);
		if constexpr (std::convertible_to<Callback, function_pointer>) {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					reinterpret_cast<function_pointer>(callback)(zstring_view_zspan{paths});
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(callback)),
				default_path,
				true
			);
			// clang-format on
		}
		else if constexpr (requires { static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator()); }) {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					reinterpret_cast<function_pointer>(callback)(zstring_view_zspan{paths});
				},
				reinterpret_cast<void*>(static_cast<function_pointer>(&std::remove_cvref_t<Callback>::operator())),
				default_path,
				true
			);
			// clang-format on
		}
		else {
			// clang-format off
			internal::show_open_folder_dialog(
				[](void* callback, const char* const* paths, int) {
					(*static_cast<std::remove_cvref_t<Callback>*>(callback))(zstring_view_zspan{paths});
					delete static_cast<std::remove_cvref_t<Callback>*>(callback);
				},
				new std::remove_cvref_t<Callback>{std::forward<Callback>(callback)},
				default_path,
				true
			);
			// clang-format on
		}
	}

	/// @}
} // namespace tr