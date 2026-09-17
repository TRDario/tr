/// @file
/// @brief Implements dialog.hpp.

#include "internal/file_dialog.hpp"
#include <SDL3/SDL.h>
#include <tr/sysgfx/dialog.hpp>

using namespace std::chrono_literals;

//

tr::message_box_button tr::show_message_box(message_box_type type, message_box_layout buttons, zstring_view title, zstring_view message)
{
	/// Button layout for the YES/NO message box.
	constexpr std::array<SDL_MessageBoxButtonData, 2> yes_no_buttons{{
		{.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, .buttonID = 0, .text = "Yes"},
		{.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, .buttonID = 1, .text = "No"},
	}};

	/// Button layout for the YES/NO/CANCEL message box.
	constexpr std::array<SDL_MessageBoxButtonData, 3> yes_no_cancel_buttons{{
		{.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, .buttonID = 0, .text = "Yes"},
		{.flags = 0, .buttonID = 1, .text = "No"},
		{.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, .buttonID = 2, .text = "Cancel"},
	}};

	const SDL_MessageBoxFlags flags{std::to_underlying(type)};

	switch (buttons) {
	case message_box_layout::ok:
		SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), nullptr);
		return message_box_button::ok;
	case message_box_layout::yes_no: {
		int selected{std::to_underlying(message_box_button::no)};
		SDL_MessageBoxData data{flags, nullptr, title.c_str(), message.c_str(), 2, yes_no_buttons.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return static_cast<message_box_button>(selected);
	}
	case message_box_layout::yes_no_cancel: {
		int selected{std::to_underlying(message_box_button::cancel)};
		SDL_MessageBoxData data{flags, nullptr, title.c_str(), message.c_str(), 3, yes_no_cancel_buttons.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return static_cast<message_box_button>(selected);
	}
	default:
		return message_box_button::ok;
	}
}

std::filesystem::path tr::show_open_file_dialog(std::span<const dialog_filter> filters, zstring_view default_path)
{
	std::vector<std::filesystem::path> vec{internal::show_open_file_dialog(filters, default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::show_open_files_dialog(std::span<const dialog_filter> filters, zstring_view default_path)
{
	return internal::show_open_file_dialog(filters, default_path, true);
}

std::filesystem::path tr::show_open_folder_dialog(zstring_view default_path)
{
	std::vector<std::filesystem::path> vec{internal::show_open_folder_dialog(default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::show_open_folders_dialog(zstring_view default_path)
{
	return internal::show_open_folder_dialog(default_path, true);
}

std::filesystem::path tr::show_save_file_dialog(std::span<const dialog_filter> filters, zstring_view default_path)
{
	internal::file_dialog_context ctx{};
	SDL_ShowSaveFileDialog(internal::file_dialog_callback, &ctx, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   filters.size(), default_path.c_str());
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return ctx.paths.empty() ? std::filesystem::path{} : std::move(ctx.paths.front());
}