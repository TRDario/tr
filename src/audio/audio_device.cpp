/// @file
/// @brief Implements audio_device.hpp.

#include <AL/alc.h>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/exception.hpp>
#include <tr/utility/macro.hpp>

//

tr::audio_device::audio_device(zstring_view name)
	: m_ptr{alcOpenDevice(name.c_str())}
{
	if (m_ptr == nullptr) {
		throw audio_device_open_error{};
	}
}

void tr::audio_device::deleter::operator()(ALCdevice* device) noexcept
{
#ifdef TR_ENABLE_ASSERTS
	TR_ASSERT(alcCloseDevice(device), "Tried to close audio device '{}' which still has active contexts.",
			  alcGetString(device, ALC_DEVICE_SPECIFIER));
#else
	alcCloseDevice(device);
#endif
}

//

tr::zstring_view tr::audio_device::name() const noexcept
{
	return alcGetString(unwrap(), ALC_DEVICE_SPECIFIER);
}

//

ALCdevice* tr::audio_device::unwrap() const noexcept
{
	return m_ptr.get();
}