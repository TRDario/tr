#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

namespace tr::sys {
	// Button layout for the YES/NO message box.
	constexpr std::array<SDL_MessageBoxButtonData, 2> YES_NO_BUTTONS{{
		{.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, .buttonID = 0, .text = "Yes"},
		{.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, .buttonID = 1, .text = "No"},
	}};
	// Button layout for the YES/NO/CANCEL message box.
	constexpr std::array<SDL_MessageBoxButtonData, 3> YES_NO_CANCEL_BUTTONS{{
		{.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, .buttonID = 0, .text = "Yes"},
		{.flags = 0, .buttonID = 1, .text = "No"},
		{.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, .buttonID = 2, .text = "Cancel"},
	}};

	// File dialog callback context.
	struct file_dialog_context {
		// List of selected file paths.
		std::vector<std::filesystem::path> paths;
		// Whether the dialog is done.
		bool done{false};
	};
	// The file dialog callback function.
	void file_dialog_callback(void* userdata, const char* const* files, int);

	// Base open file dialog function.
	std::vector<std::filesystem::path> show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path,
															 bool allow_multiple);
	// Base open folder dialog function.
	std::vector<std::filesystem::path> show_open_folder_dialog(const char* default_path, bool allow_multiple);

} // namespace tr::sys

void tr::sys::file_dialog_callback(void* userdata, const char* const* files, int)
{
	file_dialog_context& context{*(file_dialog_context*)userdata};
	if (files != nullptr) {
		while (*files != nullptr) {
			context.paths.emplace_back(*files++);
		}
	}
	context.done = true;
}

std::vector<std::filesystem::path> tr::sys::show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path,
																  bool allow_multiple)
{
	file_dialog_context ctx{};
	SDL_ShowOpenFileDialog(file_dialog_callback, &ctx, nullptr, (const SDL_DialogFileFilter*)filters.data(), int(filters.size()),
						   default_path, allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}

std::vector<std::filesystem::path> tr::sys::show_open_folder_dialog(const char* default_path, bool allow_multiple)
{
	file_dialog_context ctx{};
	SDL_ShowOpenFolderDialog(file_dialog_callback, &ctx, nullptr, default_path, allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}

tr::sys::msg_button tr::sys::show_message_box(msg_box_type type, msg_buttons buttons, const char* title, const char* message)
{
	const SDL_MessageBoxFlags flags{SDL_MessageBoxFlags(type)};

	switch (buttons) {
	case msg_buttons::OK:
		SDL_ShowSimpleMessageBox(flags, title, message, nullptr);
		return msg_button::OK;
	case msg_buttons::YES_NO: {
		int selected{int(msg_button::NO)};
		SDL_MessageBoxData data{flags, nullptr, title, message, 2, YES_NO_BUTTONS.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return msg_button(selected);
	}
	case msg_buttons::YES_NO_CANCEL: {
		int selected{int(msg_button::CANCEL)};
		SDL_MessageBoxData data{flags, nullptr, title, message, 3, YES_NO_CANCEL_BUTTONS.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return msg_button(selected);
	}
	default:
		return msg_button::OK;
	}
}

void tr::sys::show_fatal_error_message_box(const std::exception& exception)
{
	if (dynamic_cast<const std::bad_alloc*>(&exception) || dynamic_cast<const out_of_memory*>(&exception)) {
		emergency_buffer.reset();
	}

	const std::string title{app_name != nullptr ? TR_FMT::format("{} - Fatal Error", app_name) : "Fatal Error"};

	const tr::exception* tr_exception{dynamic_cast<const tr::exception*>(&exception)};
	std::string message;
	if (tr_exception) {
		message = TR_FMT::format("A fatal error has occurred ({}).", tr_exception->name());
		const std::string_view description{tr_exception->description()};
		if (!description.empty()) {
			message.push_back('\n');
			message.append(description);
		}
		const std::string_view details{tr_exception->details()};
		if (!details.empty()) {
			message.push_back('\n');
			message.append(details);
		}
		TR_LOG(log, tr::severity::FATAL, *tr_exception);
	}
	else {
		message = TR_FMT::format("A fatal error has occurred ({}).", exception.what());
		TR_LOG(log, tr::severity::FATAL, exception);
	}
	message.append("\nPress OK to exit the application.");
	show_message_box(msg_box_type::ERROR, msg_buttons::OK, title.c_str(), message.c_str());
}

std::filesystem::path tr::sys::show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	std::vector<std::filesystem::path> vec{show_open_file_dialog(filters, default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::sys::show_open_files_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	return show_open_file_dialog(filters, default_path, true);
}

std::filesystem::path tr::sys::show_open_folder_dialog(const char* default_path)
{
	std::vector<std::filesystem::path> vec{show_open_folder_dialog(default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::sys::show_open_folders_dialog(const char* default_path)
{
	return show_open_folder_dialog(default_path, true);
}

std::filesystem::path tr::sys::show_save_file_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	file_dialog_context ctx{};
	SDL_ShowSaveFileDialog(file_dialog_callback, &ctx, nullptr, (const SDL_DialogFileFilter*)filters.data(), int(filters.size()),
						   default_path);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return ctx.paths.empty() ? std::filesystem::path{} : std::move(ctx.paths.front());
}