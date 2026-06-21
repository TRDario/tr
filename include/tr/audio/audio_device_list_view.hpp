///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio device list view.                                                                                                   //
//                                                                                                                                       //
// A list of available audio devices may be obtained with tr::available_audio_devices(). These names may be used in the constructor of   //
// tr::audio_device. The list models forward_range:                                                                                      //
//     - tr::available_audio_devices() -> {"foo", "bar", "baz"}                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/zstring_view.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Audio device list view iterator.
	class audio_device_list_view_iterator {
	  public:
		// Constructs an iterator.
		audio_device_list_view_iterator(tr::zstring_view view);

		using value_type = tr::zstring_view;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		// Compares two audio device list iterators.
		friend std::strong_ordering operator<=>(audio_device_list_view_iterator l, audio_device_list_view_iterator r);
		// Compares two audio device list iterators for equality.
		friend bool operator==(audio_device_list_view_iterator l, audio_device_list_view_iterator r);

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator.
		const_pointer operator->() const;

		// Pre-increments the iterator.
		audio_device_list_view_iterator& operator++();
		// Post-increments the iterator.
		audio_device_list_view_iterator operator++(int);

	  private:
		// Null-terminated string view to the current audio device name.
		tr::zstring_view m_view;
	};

	// Audio device list end sentinel.
	class audio_device_list_view_end_sentinel {
		// Compares an audio device list iterator to an end sentinel.
		friend bool operator==(audio_device_list_view_iterator it, audio_device_list_view_end_sentinel);
	};

	// View holding available audio device names.
	using audio_device_list_view = std::ranges::subrange<audio_device_list_view_iterator, audio_device_list_view_end_sentinel>;

	// Gets a view to the list of available audio devices.
	audio_device_list_view available_audio_devices();
} // namespace tr