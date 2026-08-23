/// @file
/// @brief Provides an RAII handle class and related utilities.

#pragma once
#include "common.hpp"

//

namespace tr
{
	/// Valid handle deleter type for a handle to `Base`.
	template <typename T, typename Base>
	concept handle_deleter = std::invocable<T, Base> && (std::move_constructible<T> || std::copy_constructible<T>);

	/// Default constructible handle deleter.
	template <typename T>
	concept default_constructible_handle_deleter = std::default_initializable<T> && !std::is_pointer_v<T>;

	//

	/// Tag struct used in some handle functions to suppress empty value checking.
	struct maybe_empty_t
	{
	};

	/// Tag value used in some handle functions to suppress empty value checking.
	constexpr maybe_empty_t maybe_empty{};

	//

	/// RAII wrapper over non-pointer handles.
	/// @tparam Base Wrapped type.
	/// @tparam Empty Empty handle sentinel value.
	/// @tparam Deleter Deleter invoked when destroying a handled value.
	template <std::regular Base, Base Empty, handle_deleter<Base> Deleter>
	class handle : private Deleter
	{
	  public:
		/// @name Constructors
		/// @{

		/// Default-constructs an empty handle.
		constexpr handle()
			requires(default_constructible_handle_deleter<Deleter>);

		/// Constructs a handle from a base type value.
		/// @param value Value to hold.
		constexpr explicit handle(Base value)
			requires(default_constructible_handle_deleter<Deleter>);

		/// Constructs a handle from a base type value without checking for the invalid case.
		/// @param value Value to hold.
		constexpr explicit handle(Base value, maybe_empty_t)
			requires(default_constructible_handle_deleter<Deleter>);

		/// Default-constructs an empty handle.
		/// @param deleter Deleter instance.
		constexpr handle(Deleter&& deleter);

		/// Constructs a handle from a base type value and a deleter.
		/// @pre `value` must not be equal to `Empty`.
		/// @param value Value to hold.
		/// @param deleter Deleter instance.
		constexpr explicit handle(Base value, Deleter&& deleter)
			requires(std::move_constructible<Deleter>);

		/// Constructs a handle from a base type value and a deleter without checking for the invalid case.
		/// @param value Value to hold.
		/// @param deleter Deleter instance.
		constexpr explicit handle(Base value, Deleter&& deleter, maybe_empty_t)
			requires(std::move_constructible<Deleter>);

		/// Constructs a handle by moving from another handle.
		/// @param rhs Handle to move.
		constexpr handle(handle&& rhs) noexcept
			requires(std::move_constructible<Deleter>);

		/// Destroys the handle.
		constexpr ~handle();

		/// @}
		/// @name Assignment operators
		/// @{

		/// Move-assigns the handle.
		/// @param rhs Handle to move.
		/// @return Reference to `*this`.
		constexpr handle& operator=(handle&& rhs) noexcept
			requires(std::is_move_assignable_v<Deleter>);

		/// @}
		/// @name State
		/// @{

		/// Checks if the handle contains a value.
		/// @return `true` if the handle contains a value, `false` otherwise.
		constexpr bool has_value() const;

		/// Checks if the handle contains a value.
		/// @return `true` if the handle contains a value, `false` otherwise.
		constexpr explicit operator bool() const;

		/// @}
		/// @name Getters
		/// @{

		/// Gets the handle's base type value.
		/// @pre The handle must contain a value.
		/// @return Reference to the contained value.
		constexpr const Base& get() const;

		/// Gets the handle's base type value without checking for the invalid case.
		/// @return Reference to the contained value.
		constexpr const Base& get(maybe_empty_t) const;

		/// Gets the handle's deleter.
		/// @return Reference to the handle's deleter.
		constexpr Deleter& get_deleter();

		/// Gets the handle's deleter.
		/// @return Reference to the handle's deleter.
		constexpr const Deleter& get_deleter() const;

		/// @}
		/// @name Resetting
		/// @{

		/// Releases ownership over the handle, if any.
		/// @return Previously contained value.
		constexpr Base release();

		/// Resets the handle to an empty state.
		constexpr void reset();

		/// Resets the handle to a non-empty state.
		/// @pre `value` must not be equal to `Empty`.
		/// @param value New value to hold.
		constexpr void reset(Base value);

		/// Resets the handle to a new state without checking for the invalid case.
		/// @param value New value to hold.
		constexpr void reset(Base value, maybe_empty_t);

		/// @}

	  private:
		/// Wrapped base value.
		Base m_base;
	};

	//

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
		out_handle_t(handle<Base, Empty, Deleter>& handle);

		/// Sets the handle.
		~out_handle_t();

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a pointer that can be used by a function writing a base value.
		/// @return Pointer that can be used by a function writing a base value.
		operator Base*();

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
	out_handle_t<Base, Empty, Deleter, false> out_handle(handle<Base, Empty, Deleter>& handle);

	/// Wraps a handle for use in functions that output using a pointer, akin to std::out_ptr.
	/// @tparam Base Handle base type.
	/// @tparam Empty Empty handle sentinel value.
	/// @tparam Deleter Handle deleter type.
	/// @param handle Handle to wrap.
	/// @return Wrapped output handle.
	template <std::regular Base, Base Empty, handle_deleter<Base> Deleter>
	out_handle_t<Base, Empty, Deleter, true> out_handle(handle<Base, Empty, Deleter>& handle, maybe_empty_t);

	/// @}
} // namespace tr

#include "impl/handle.hpp" // IWYU pragma: export