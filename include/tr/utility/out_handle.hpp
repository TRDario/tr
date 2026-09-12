/// @file
/// @brief Provides `tr::out_handle()`.

#pragma once
#include <tr/utility/handle.hpp>

//

namespace tr
{
	/// Wrapper type returned by out_handle.
	/// @tparam Base Handle base type.
	/// @tparam Empty Empty handle sentinel value.
	/// @tparam Deleter Handle deleter type.
	/// @tparam SkipEmptyHandleCheck Whether to skip the empty handle check when constructing the wrapper.
	template <std::regular Base, Base Empty, handle_deleter<Base> Deleter, bool SkipEmptyHandleCheck>
	class out_handle_t
	{
	  public:
		/// @name Constructors
		/// @{

		/// Wraps an output handle.
		/// @param handle Handle to wrap.
		[[nodiscard]] out_handle_t(handle<Base, Empty, Deleter>& handle) noexcept
			: m_handle{handle}
		{
		}

		/// Sets the handle.
		~out_handle_t() noexcept
		{
			if constexpr (SkipEmptyHandleCheck) {
				m_handle.reset(m_temporary, maybe_empty);
			}
			else {
				m_handle.reset(m_temporary);
			}
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a pointer that can be used by a function writing a base value.
		/// @return Pointer that can be used by a function writing a base value.
		[[nodiscard]] operator Base*() noexcept
		{
			return &m_temporary;
		}

		/// @}

	  private:
		/// Handle being modified.
		handle<Base, Empty, Deleter>& m_handle;

		/// Temporary that is written to before the handle is set to it.
		Base m_temporary{Empty};
	};

	/// @name Output handle
	/// @{

	/// Wraps a handle for use in functions that output using a pointer, akin to std::out_ptr.
	/// @tparam Base Handle base type.
	/// @tparam Empty Empty handle sentinel value.
	/// @tparam Deleter Handle deleter type.
	/// @param handle Handle to wrap.
	/// @return Wrapped output handle.
	template <std::regular Base, Base Empty, handle_deleter<Base> Deleter>
	[[nodiscard]] out_handle_t<Base, Empty, Deleter, false> out_handle(handle<Base, Empty, Deleter>& handle) noexcept
	{
		return handle;
	}

	/// Wraps a handle for use in functions that output using a pointer, akin to std::out_ptr.
	/// @tparam Base Handle base type.
	/// @tparam Empty Empty handle sentinel value.
	/// @tparam Deleter Handle deleter type.
	/// @param handle Handle to wrap.
	/// @return Wrapped output handle.
	template <std::regular Base, Base Empty, handle_deleter<Base> Deleter>
	[[nodiscard]] out_handle_t<Base, Empty, Deleter, true> out_handle(handle<Base, Empty, Deleter>& handle, maybe_empty_t) noexcept
	{
		return handle;
	}

	/// @}
} // namespace tr