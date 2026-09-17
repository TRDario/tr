/// @file
/// @brief Implements internal/file_dialog.hpp.

#include "internal/file_dialog.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

//

void tr::internal::file_dialog_callback(void* userdata, const char* const* files, int)
{
	file_dialog_context& context{*static_cast<file_dialog_context*>(userdata)};
	if (files != nullptr) {
		while (*files != nullptr) {
			context.paths.emplace_back(*files++);
		}
	}
	context.done = true;
}

std::vector<std::filesystem::path> tr::internal::show_open_file_dialog(std::span<const tr::dialog_filter> filters,
																	   zstring_view default_path, bool allow_multiple)
{
	file_dialog_context ctx{};
	SDL_ShowOpenFileDialog(file_dialog_callback, &ctx, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   filters.size(), default_path.c_str(), allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}

std::vector<std::filesystem::path> tr::internal::show_open_folder_dialog(zstring_view default_path, bool allow_multiple)
{
	file_dialog_context ctx{};
	SDL_ShowOpenFolderDialog(file_dialog_callback, &ctx, nullptr, default_path.c_str(), allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}