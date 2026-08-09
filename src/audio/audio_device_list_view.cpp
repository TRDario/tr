/// @file
/// @brief Implements audio_device_list_view.hpp.

#include "../../include/tr/audio/audio_device_list_view.hpp"
#include "../../include/tr/utility/macro.hpp"
#include <AL/alc.h>

//

tr::audio_device_list_view_iterator::audio_device_list_view_iterator(tr::zstring_view view)
	: m_view{view}
{
}

//

std::strong_ordering tr::operator<=>(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs)
{
	return lhs->c_str() <=> rhs->c_str();
}

bool tr::operator==(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs)
{
	return lhs->c_str() == rhs->c_str();
}

//

tr::audio_device_list_view_iterator::value_type tr::audio_device_list_view_iterator::operator*() const
{
	return m_view;
}

tr::audio_device_list_view_iterator::const_pointer tr::audio_device_list_view_iterator::operator->() const
{
	return &m_view;
}

//

tr::audio_device_list_view_iterator& tr::audio_device_list_view_iterator::operator++()
{
	TR_ASSERT(!m_view.empty(), "Tried to increment audio device list end iterator.");
	m_view.remove_prefix(m_view.length() + 1);
	return *this;
}

tr::audio_device_list_view_iterator tr::audio_device_list_view_iterator::operator++(int)
{
	audio_device_list_view_iterator copy{*this};
	++*this;
	return copy;
}

//

bool tr::operator==(audio_device_list_view_iterator it, audio_device_list_view_end_sentinel)
{
	return it->empty();
}

//

tr::audio_device_list_view tr::available_audio_devices()
{
	return {audio_device_list_view_iterator{alcGetString(nullptr, ALC_DEVICE_SPECIFIER)}, audio_device_list_view_end_sentinel{}};
}