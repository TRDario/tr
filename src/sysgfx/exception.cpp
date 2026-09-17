/// @file
/// @brief Implements sysgfx/exception.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/exception.hpp>

//

tr::bitmap_load_error::bitmap_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load bitmap from '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::bitmap_load_error::name() const noexcept
{
	return "Bitmap loading error";
}

std::string_view tr::bitmap_load_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::bitmap_load_error::details() const noexcept
{
	return m_details;
}

//

tr::bitmap_save_error::bitmap_save_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to save bitmap to '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::bitmap_save_error::name() const noexcept
{
	return "Bitmap saving error";
}

std::string_view tr::bitmap_save_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::bitmap_save_error::details() const noexcept
{
	return m_details;
}

//

tr::cursor_error::cursor_error(std::string_view description) noexcept
	: m_description{description}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::cursor_error::name() const noexcept
{
	return "Cursor error";
}

std::string_view tr::cursor_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::cursor_error::details() const noexcept
{
	return m_details;
}

//

tr::graphics_context_init_error::graphics_context_init_error()
	: m_description{SDL_GetError()}
{
}

std::string_view tr::graphics_context_init_error::name() const noexcept
{
	return "Graphics context opening error";
}

std::string_view tr::graphics_context_init_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::graphics_context_init_error::details() const noexcept
{
	return {};
}

//

tr::path_error::path_error(std::string_view description) noexcept
	: m_description{description}
{
}

std::string_view tr::path_error::name() const noexcept
{
	return "Path error";
}

std::string_view tr::path_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::path_error::details() const noexcept
{
	return {};
}

//

tr::set_clipboard_error::set_clipboard_error() noexcept
	: m_description{SDL_GetError()}
{
}

std::string_view tr::set_clipboard_error::name() const noexcept
{
	return "Clipboard setting error";
}

std::string_view tr::set_clipboard_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::set_clipboard_error::details() const noexcept
{
	return {};
}

//

tr::shader_load_error::shader_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load shader from '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::shader_load_error::name() const noexcept
{
	return "Shader loading error";
}

std::string_view tr::shader_load_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::shader_load_error::details() const noexcept
{
	return m_details;
}

//

tr::ttfont_load_error::ttfont_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load bitmap from '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::ttfont_load_error::name() const noexcept
{
	return "TrueType font loading error";
}

std::string_view tr::ttfont_load_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::ttfont_load_error::details() const noexcept
{
	return m_details;
}

//

tr::ttfont_render_error::ttfont_render_error(std::string_view description) noexcept
	: m_description{description}
{
}

std::string_view tr::ttfont_render_error::name() const noexcept
{
	return "TrueType font rendering error";
}

std::string_view tr::ttfont_render_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::ttfont_render_error::details() const noexcept
{
	return {};
}

//

tr::window_error::window_error(std::string&& description) noexcept
	: m_description{description}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::window_error::name() const noexcept
{
	return "Window error";
}

std::string_view tr::window_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::window_error::details() const noexcept
{
	return m_details;
}

//

tr::window_open_error::window_open_error() noexcept
	: m_description{SDL_GetError()}
{
}

std::string_view tr::window_open_error::name() const noexcept
{
	return "Window opening error";
}

std::string_view tr::window_open_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::window_open_error::details() const noexcept
{
	return {};
}