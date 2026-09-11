/// @file
/// @brief Provides an audio device list view.

#pragma once
#include "../utility/zstring_view.hpp"

//

namespace tr
{
	/// Audio device list view iterator.
	class audio_device_list_view_iterator
	{
	  public:
		/// Value type used by the iterator.
		using value_type = zstring_view;

		/// Pointer type used by the iterator.
		using pointer = value_type*;

		/// Constant pointer type used by the iterator.
		using const_pointer = const value_type*;

		/// Reference type used by the iterator.
		using reference = value_type&;

		/// Constant reference type used by the iterator.
		using const_reference = const value_type&;

		/// Size type used by the iterator.
		using size_type = std::size_t;

		/// Differenec type used by the iterator.
		using difference_type = std::ptrdiff_t;

		/// @name Constructors
		/// @{

		/// Constructs an iterator.
		/// @param view NUL-terminated string view to an audio device name.
		[[nodiscard]] audio_device_list_view_iterator(zstring_view view) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two audio device list iterators.
		/// @param lhs, rhs Iterators to compare.
		/// @return Ordering of the iterators.
		friend std::strong_ordering operator<=>(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs) noexcept;

		/// Compares two audio device list iterators for equality.
		/// @param lhs, rhs Iterators to compare.
		/// @return Whether the iterators are equal.
		friend bool operator==(audio_device_list_view_iterator lhs, audio_device_list_view_iterator rhs) noexcept;

		/// @}
		/// @name Other operators
		/// @{

		/// Dereferences the iterator.
		[[nodiscard]] value_type operator*() const noexcept;

		/// Dereferences the iterator.
		[[nodiscard]] const_pointer operator->() const noexcept;

		/// Pre-increments the iterator.
		audio_device_list_view_iterator& operator++() noexcept;

		/// Post-increments the iterator.
		audio_device_list_view_iterator operator++(int) noexcept;

		/// @}

	  private:
		/// NUL-terminated string view to the current audio device name.
		zstring_view m_view;
	};

	/// Audio device list end sentinel.
	class audio_device_list_view_end_sentinel
	{
		/// Compares an audio device list iterator to an end sentinel.
		/// @return `true` if the iterator is at the end of the range, false otherwise.
		friend bool operator==(audio_device_list_view_iterator it, audio_device_list_view_end_sentinel) noexcept;
	};

	/// View holding available audio device names.
	using audio_device_list_view = std::ranges::subrange<audio_device_list_view_iterator, audio_device_list_view_end_sentinel>;

	/// @name Audio devices
	/// @{

	/// Gets a view to the list of available audio devices.
	/// @return List of valid audio device names.
	[[nodiscard]] audio_device_list_view available_audio_devices() noexcept;

	/// @}
} // namespace tr