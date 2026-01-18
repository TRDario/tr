///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an RAII handle class.                                                                                                        //
//                                                                                                                                       //
// Handles take ownership over a resource managed by a simple handle (like an integer) and automatically clean them up with the          //
// appropriate deleter. One value of the underlying type is reserved for an 'empty' value that won't be cleaned up and is used as the    //
// default. Construction and setting will consider setting the handle to the empty value an error by default, as will getters when       //
// getting from an empty handle, but this can be suppressed by using tr::no_empty_handle_check as an extra parameter:                    //
//     - tr::handle<int, 0, deleter> -> 0 indicates an empty handle, deleter::operator() is used to clean up the handle                  //
//     - tr::handle<int, 0, deleter>{} -> empty handle                                                                                   //
//     - tr::handle<int, 0, deleter>{0} -> ERROR!                                                                                        //
//     - tr::handle<int, 0, deleter>{0, tr::no_empty_handle_check} -> empty handle                                                       //
//     - tr::handle<int, 0, deleter>{5} -> non-empty handle                                                                              //
//                                                                                                                                       //
// Whether a handle has a value can be checked with operator bool or the .has_value() method, while the value can be gotten with .get(): //
//     - tr::handle<int, 0, deleter>{}.has_value() -> false                                                                              //
//     - tr::handle<int, 0, deleter>{}.get() -> ERROR!                                                                                   //
//     - tr::handle<int, 0, deleter>{}.get(tr::no_empty_handle_check) -> 0                                                               //
//     - tr::handle<int, 0, deleter>{5}.has_value() -> true                                                                              //
//     - tr::handle<int, 0, deleter>{5}.get() -> 5                                                                                       //
//                                                                                                                                       //
// The value of the handle can be released, reset, or swapped:                                                                           //
//     - tr::handle<int, 0, deleter> handle{5}; handle.release() -> 5, handle is now empty                                               //
//     - tr::handle<int, 0, deleter> handle{5}; handle.reset() -> deleter called, handle is now empty                                    //
//     - tr::handle<int, 0, deleter> handle{5}; handle.reset(4) -> deleter called, handle now holds 4                                    //
//     - tr::handle<int, 0, deleter> handle{5}; handle.reset(0) -> ERROR!                                                                //
//     - tr::handle<int, 0, deleter> handle{5}; handle.reset(0, tr::no_empty_handle_check) -> deleter called, handle is now empty        //
//     - handle.swap(handle2) -> 'handle' and 'handle2' now hold eachother's value                                                       //
//                                                                                                                                       //
// Non-trivial destructors are supported: they can be passed in the constructor, or gotten with .get_deleter().                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Concept that denotes a valid handle deleter type for a handle of base type @em H.
	template <class T, class H>
	concept handle_deleter = std::invocable<T, H> && (std::move_constructible<T> || std::copy_constructible<T>);
	// Concept that denotes a default constructible handle deleter.
	template <class T>
	concept default_constructible_handle_deleter = std::default_initializable<T> && !std::is_pointer_v<T>;

	// Tag struct used in some handle functions to suppress empty value checking.
	struct no_empty_handle_check_t {};
	// Tag value used in some handle functions to suppress empty value checking.
	constexpr no_empty_handle_check_t no_empty_handle_check{};

	// RAII wrapper over non-pointer handles.
	template <std::regular T, T E, handle_deleter<T> D> class handle : private D {
	  public:
		// Default-constructs an empty handle.
		constexpr handle()
			requires(default_constructible_handle_deleter<D>);
		// Constructs a handle from a base type value.
		constexpr explicit handle(T value)
			requires(default_constructible_handle_deleter<D>);
		// Constructs a handle from a base type value and a deleter.
		constexpr explicit handle(T value, D& deleter)
			requires(std::copy_constructible<D>);
		// Constructs a handle from a base type value and a deleter.
		constexpr explicit handle(T value, const D& deleter)
			requires(std::copy_constructible<D> && !(lvalue_reference<D> && !const_qualified<D>));
		// Constructs a handle from a base type value and a deleter.
		constexpr explicit handle(T value, D&& deleter)
			requires(std::move_constructible<D> && !lvalue_reference<D>);
		// Constructs a handle from a base type value without checking for the invalid E case.
		constexpr explicit handle(T value, no_empty_handle_check_t)
			requires(default_constructible_handle_deleter<D>);
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, D& deleter, no_empty_handle_check_t)
			requires(std::copy_constructible<D>);
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, const D& deleter, no_empty_handle_check_t)
			requires(std::copy_constructible<D> && !(lvalue_reference<D> && !const_qualified<D>));
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, D&& deleter, no_empty_handle_check_t)
			requires(std::move_constructible<D> && !lvalue_reference<D>);
		// Constructs a handle by moving from another handle.
		template <handle_deleter<T> D1> constexpr handle(handle<T, E, D1>&& move) noexcept;
		// Destroys the handle.
		constexpr ~handle();

		// Move-assigns the handle.
		constexpr handle& operator=(handle&& r) noexcept;

		// Checks if the handle contains a value.
		constexpr bool has_value() const;
		// Checks if the handle contains a value.
		constexpr explicit operator bool() const;

		// Gets the handle's base type value.
		constexpr const T& get() const;
		// Gets the handle's base type value.
		constexpr const T& get(no_empty_handle_check_t) const;
		// Gets the handle's deleter.
		constexpr D& get_deleter();
		// Gets the handle's deleter.
		constexpr D& get_deleter() const;

		// Releases ownership over the handle, if any.
		constexpr T release();
		// Resets the handle to an empty state.
		constexpr void reset();
		// Resets the handle to a non-empty state.
		constexpr void reset(T value);
		// Resets the handle to a new state.
		constexpr void reset(T value, no_empty_handle_check_t);

		// Swaps the ownership over values between two handles.
		constexpr void swap(handle& other);

	  private:
		// The wrapped value.
		T m_base;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

#include "handle_impl.hpp" // IWYU pragma: export