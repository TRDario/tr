/// @file
/// @brief Implements internal/file_dialog.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/file_dialog.hpp>

//

void tr::internal::show_open_file_dialog(raw_file_dialog_callback callback, void* userdata, std::span<const file_dialog_filter> filters,
										 zstring_view default_path, bool allow_multiple) noexcept
{
	SDL_ShowOpenFileDialog(callback, userdata, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   static_cast<int>(filters.size()), default_path.c_str(), allow_multiple);
}

void tr::internal::show_save_file_dialog(raw_file_dialog_callback callback, void* userdata, std::span<const file_dialog_filter> filters,
										 zstring_view default_path) noexcept
{
	SDL_ShowSaveFileDialog(callback, userdata, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   static_cast<int>(filters.size()), default_path.c_str());
}

void tr::internal::show_open_folder_dialog(raw_file_dialog_callback callback, void* userdata, zstring_view default_path,
										   bool allow_multiple) noexcept
{
	SDL_ShowOpenFolderDialog(callback, userdata, nullptr, default_path.c_str(), allow_multiple);
}