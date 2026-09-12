/// @file
/// @brief Implements audio/exception.hpp.

#include <AL/alc.h>
#include <tr/audio/exception.hpp>

//

std::string_view tr::audio_device_open_error::name() const noexcept
{
	return "Audio device opening error";
}

std::string_view tr::audio_device_open_error::description() const noexcept
{
	return {};
}

std::string_view tr::audio_device_open_error::details() const noexcept
{
	return {};
}

//

tr::audio_context_init_error::audio_context_init_error(ALCdevice* device) noexcept
	: m_description{alcGetString(device, alcGetError(device))}
{
}

std::string_view tr::audio_context_init_error::name() const noexcept
{
	return "Audio context initialization error";
}

std::string_view tr::audio_context_init_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::audio_context_init_error::details() const noexcept
{
	return {};
}

//

tr::audio_file_open_error::audio_file_open_error(std::string&& description) noexcept
	: m_description{std::move(description)}
{
}

std::string_view tr::audio_file_open_error::name() const noexcept
{
	return "Audio file opening error";
}

std::string_view tr::audio_file_open_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::audio_file_open_error::details() const noexcept
{
	return {};
}