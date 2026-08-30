/// @file
/// @brief Implements audio_device.hpp.

#include "../../include/tr/audio/audio_device.hpp"
#include "../../include/tr/utility/macro.hpp"
#include <AL/alc.h>

//

std::string_view tr::audio_device_open_error::name() const
{
	return "Audio device opening error";
}

std::string_view tr::audio_device_open_error::description() const
{
	return {};
}

std::string_view tr::audio_device_open_error::details() const
{
	return {};
}

//

tr::audio_device::audio_device(zstring_view name)
	: m_ptr{alcOpenDevice(name.c_str())}
{
	if (m_ptr == nullptr) {
		// THROW EXCEPTION
	}
}

void tr::audio_device::deleter::operator()(ALCdevice* device)
{
#ifdef TR_ENABLE_ASSERTS
	TR_ASSERT(alcCloseDevice(device), "Tried to close audio device '{}' which still has active contexts.",
			  alcGetString(device, ALC_DEVICE_SPECIFIER));
#else
	alcCloseDevice(device);
#endif
}

//

tr::zstring_view tr::audio_device::name() const
{
	return alcGetString(m_ptr.get(), ALC_DEVICE_SPECIFIER);
}