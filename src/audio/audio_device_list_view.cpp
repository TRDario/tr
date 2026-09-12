/// @file
/// @brief Implements audio_device_list_view.hpp.

#include <AL/alc.h>
#include <tr/audio/audio_device_list_view.hpp>
#include <tr/utility/macro.hpp>

//

tr::audio_device_list_view_iterator::audio_device_list_view_iterator(tr::zstring_view view) noexcept
	: m_view{view}
{
}

//

tr::audio_device_list_view_iterator::value_type tr::audio_device_list_view_iterator::operator*() const noexcept
{
	return m_view;
}

tr::audio_device_list_view_iterator::const_pointer tr::audio_device_list_view_iterator::operator->() const noexcept
{
	return &m_view;
}

//

tr::audio_device_list_view_iterator& tr::audio_device_list_view_iterator::operator++() noexcept
{
	TR_ASSERT(!m_view.empty(), "Tried to increment audio device list end iterator.");
	m_view.remove_prefix(m_view.length() + 1);
	return *this;
}

tr::audio_device_list_view_iterator tr::audio_device_list_view_iterator::operator++(int) noexcept
{
	audio_device_list_view_iterator copy{*this};
	++*this;
	return copy;
}

//

std::strong_ordering tr::operator<=>(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs) noexcept
{
	return lhs->c_str() <=> rhs->c_str();
}

bool tr::operator==(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs) noexcept
{
	return lhs->c_str() == rhs->c_str();
}

bool tr::operator==(audio_device_list_view_iterator it, audio_device_list_view_end_sentinel) noexcept
{
	return it->empty();
}

//

tr::audio_device_list_view tr::available_audio_devices() noexcept
{
	return {audio_device_list_view_iterator{alcGetString(nullptr, ALC_DEVICE_SPECIFIER)}, audio_device_list_view_end_sentinel{}};
}