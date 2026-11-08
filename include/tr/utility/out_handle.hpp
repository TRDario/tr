/// @file
/// @brief Provides `tr::out_handle()`.

#pragma once

//

namespace tr
{
	/// @ingroup handle
	/// @brief Wrapper over a smart handle for use in functions that output using a pointer, akin to `std::out_ptr`.
	/// @tparam SmartHandle Smart handle type being wrapped.
	template <typename SmartHandle>
		requires requires(SmartHandle& smart) {
			typename SmartHandle::handle_type;
			SmartHandle::empty_sentinel;
			smart.reset(typename SmartHandle::handle_type{});
		}
	class out_handle
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Wraps an output handle.
		/// @param handle Handle to wrap.
		[[nodiscard]] out_handle(SmartHandle& handle) noexcept
			: m_handle{handle}
		{
		}

		/// @brief Sets the handle.
		~out_handle() noexcept
		{
			m_handle.reset(m_temporary);
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns a pointer that can be used by a function writing a handle.
		/// @return Pointer that can be used by a function writing a handle.
		[[nodiscard]] operator SmartHandle::handle_type*() noexcept
		{
			return &m_temporary;
		}

		/// @}
	  private:
		/// @brief Smart handle being modified.
		SmartHandle& m_handle;

		/// @brief Temporary that is written to before the smart handle is set to it.
		typename SmartHandle::handle_type m_temporary{SmartHandle::empty_sentinel};
	};
} // namespace tr