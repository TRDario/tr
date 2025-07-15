#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

namespace tr {
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
	struct _file_dialog_context {
		// List of selected file paths.
		std::vector<std::filesystem::path> paths;
		// Whether the dialog is done.
		bool done{false};
	};
	// The file dialog callback function.
	void _file_dialog_callback(void* userdata, const char* const* files, int) noexcept;

	// Base open file dialog function.
	std::vector<std::filesystem::path> _show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path,
															  bool allow_multiple);
	// Base open folder dialog function.
	std::vector<std::filesystem::path> _show_open_folder_dialog(const char* default_path, bool allow_multiple);

} // namespace tr

void tr::_file_dialog_callback(void* userdata, const char* const* files, int) noexcept
{
	_file_dialog_context& context{*static_cast<_file_dialog_context*>(userdata)};
	if (files != nullptr) {
		while (*files != nullptr) {
			try {
				context.paths.emplace_back(*files++);
			}
			catch (std::bad_alloc&) {
				terminate("Out of memory", "Exception occurred while allocating a file path.");
			}
		}
	}
	context.done = true;
}

std::vector<std::filesystem::path> tr::_show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path,
															  bool allow_multiple)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to open a file dialog before initializing the application.");

	_file_dialog_context ctx{};
	SDL_ShowOpenFileDialog(_file_dialog_callback, &ctx, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   static_cast<int>(filters.size()), default_path, allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}

std::vector<std::filesystem::path> tr::_show_open_folder_dialog(const char* default_path, bool allow_multiple)
{
	TR_ASSERT(SDL_WasInit(0), "Tried to open a folder dialog before initializing the application.");

	_file_dialog_context ctx{};
	SDL_ShowOpenFolderDialog(_file_dialog_callback, &ctx, nullptr, default_path, allow_multiple);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return std::move(ctx.paths);
}

tr::msg_button tr::show_message_box(msg_box_type type, msg_buttons buttons, const char* title, const char* message) noexcept
{
	const SDL_MessageBoxFlags flags{static_cast<SDL_MessageBoxFlags>(type)};

	switch (buttons) {
	case msg_buttons::OK:
		SDL_ShowSimpleMessageBox(flags, title, message, nullptr);
		return msg_button::OK;
	case msg_buttons::YES_NO: {
		int selected{static_cast<int>(msg_button::NO)};
		SDL_MessageBoxData data{flags, nullptr, title, message, 2, YES_NO_BUTTONS.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return static_cast<msg_button>(selected);
	}
	case msg_buttons::YES_NO_CANCEL: {
		int selected{static_cast<int>(msg_button::CANCEL)};
		SDL_MessageBoxData data{flags, nullptr, title, message, 3, YES_NO_CANCEL_BUTTONS.data(), nullptr};
		SDL_ShowMessageBox(&data, &selected);
		return static_cast<msg_button>(selected);
	}
	default:
		return msg_button::OK;
	}
}

void tr::show_fatal_error_message_box(const exception& exception) noexcept
{
	try {
		std::string message{std::format("A fatal error has occurred ({}).", exception.name())};
		const std::string_view description{exception.description()};
		if (!description.empty()) {
			message.push_back('\n');
			message.append(description);
		}
		const std::string_view details{exception.details()};
		if (!details.empty()) {
			message.push_back('\n');
			message.append(details);
		}
		message.append("\nPress OK to exit the application.");
		const std::string title{_app_name != nullptr ? std::format("{} - Fatal Error", _app_name) : "Fatal Error"};
		show_message_box(msg_box_type::ERROR, msg_buttons::OK, title.c_str(), message.c_str());
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while creating a fatal error message box.");
	}
}

void tr::show_fatal_error_message_box(const std::exception& exception) noexcept
{
	try {
		std::string message{std::format("A fatal error has occurred ({}).\nPress OK to exit the application.", exception.what())};
		const std::string title{_app_name != nullptr ? std::format("{} - Fatal Error", _app_name) : "Fatal Error"};
		show_message_box(msg_box_type::ERROR, msg_buttons::OK, title.c_str(), message.c_str());
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while creating a fatal error message box.");
	}
}

std::filesystem::path tr::show_open_file_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	std::vector<std::filesystem::path> vec{_show_open_file_dialog(filters, default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::show_open_files_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	return _show_open_file_dialog(filters, default_path, true);
}

std::filesystem::path tr::show_open_folder_dialog(const char* default_path)
{
	std::vector<std::filesystem::path> vec{_show_open_folder_dialog(default_path, false)};
	return vec.empty() ? std::filesystem::path{} : std::move(vec.front());
}

std::vector<std::filesystem::path> tr::show_open_folders_dialog(const char* default_path)
{
	return _show_open_folder_dialog(default_path, true);
}

std::filesystem::path tr::show_save_file_dialog(std::span<const dialog_filter> filters, const char* default_path)
{
	_file_dialog_context ctx{};
	SDL_ShowSaveFileDialog(_file_dialog_callback, &ctx, nullptr, reinterpret_cast<const SDL_DialogFileFilter*>(filters.data()),
						   static_cast<int>(filters.size()), default_path);
	while (!ctx.done) {
		SDL_PumpEvents();
		std::this_thread::sleep_for(10ms);
	}
	return ctx.paths.empty() ? std::filesystem::path{} : std::move(ctx.paths.front());
}

void tr::terminate(std::string_view reason, std::string_view details) noexcept
{
	try {
		TR_LOG(log, severity::FATAL, "{}, terminating.", reason);
		TR_LOG_CONTINUE(log, "{}", details);

		std::string message{std::format("A fatal error has occurred ({}).", reason)};
		if (!details.empty()) {
			message.push_back('\n');
			message.append(details);
		}
		message.append("\nPress OK to exit the application.");
		const std::string title{_app_name != nullptr ? std::format("{} - Fatal Error", _app_name) : "Fatal Error"};
		show_message_box(msg_box_type::ERROR, msg_buttons::OK, title.c_str(), message.c_str());
		std::abort();
	}
	catch (std::bad_alloc&) {
		std::abort();
	}
}