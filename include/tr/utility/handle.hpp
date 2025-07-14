#pragma once
#include "concepts.hpp"
#include "macro.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup handle handle
	 *  Non-pointer handle RAII wrapper.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Concept that denotes a valid handle base type.
	 *
	 * To satisfy this requirement, a type must be trivially copyable and equality comparable.
	 ******************************************************************************************************************/
	template <class T>
	concept handleable = std::is_trivially_copyable_v<T> && std::equality_comparable<T>;

	/******************************************************************************************************************
	 * Concept that denotes a valid handle deleter type for a handle of base type @em H.
	 *
	 * To satisfy this requirement, a type must be invocable with @em H, be move- or copy-constructible.
	 ******************************************************************************************************************/
	template <class T, class H>
	concept handle_deleter = std::invocable<T, H> && (std::move_constructible<T> || std::copy_constructible<T>);

	/******************************************************************************************************************
	 * Concept that denotes a default constructible handle deleter.
	 *
	 * To satisfy this requirement, a type must be default initializable, but not a pointer.
	 ******************************************************************************************************************/
	template <class T>
	concept default_constructible_handle_deleter = std::default_initializable<T> && !std::is_pointer_v<T>;

	/******************************************************************************************************************
	 * Tag struct used in some handle functions to suppress empty value checking.
	 ******************************************************************************************************************/
	struct no_empty_handle_check_t {};

	/******************************************************************************************************************
	 * Tag value used in some handle functions to suppress empty value checking.
	 ******************************************************************************************************************/
	constexpr no_empty_handle_check_t NO_EMPTY_HANDLE_CHECK{};

	/******************************************************************************************************************
	 * RAII wrapper over non-pointer handles.
	 *
	 * libtr examples of usage of this type include various OpenGL and OpenAL objects.
	 *
	 * @tparam T Underlying type used by the handle, must satisfy <em>HandleType</em>.
	 * @tparam E A value of type T set aside for use as a sentinel of an empty handle.
	 * @tparam D The deleter used by the handle, must satisfy @link handle_deleter <em>handle_deleter<T></em>
	 *@endlink.
	 ******************************************************************************************************************/
	template <handleable T, T E, handle_deleter<T> D> class handle : private D {
	  public:
		/**************************************************************************************************************
		 * Default-constructs an empty handle.
		 *
		 * Requires that @em deleter is default constructible (pointers are not counted as default constructible).
		 **************************************************************************************************************/
		constexpr handle() noexcept
			requires(default_constructible_handle_deleter<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * Requires that @em deleter is default constructible (pointers are not counted as default constructible).
		 *
		 * @remark If @em E is an expected input, handle(T, no_empty_handle_check_t) may be used instead.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * @pre @em value is not allowed to equal @em E.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr explicit handle(T value) noexcept
			requires(default_constructible_handle_deleter<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * Requires that @em deleter is copy-constructible.
		 *
		 * @remark If @em E is an expected input, handle(T, deleter, no_empty_handle_check_t) may be used instead.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * @pre @em value is not allowed to equal @em E.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, D& deleter) noexcept(std::is_nothrow_copy_constructible_v<D>)
			requires(std::copy_constructible<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * Requires that @em deleter is copy-constructible.
		 *
		 * @remark If @em E is an expected input, handle(T, deleter, no_empty_handle_check_t) may be used instead.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * @pre @em value is not allowed to equal @em E.
		 * @endparblock
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, const D& deleter) noexcept(std::is_nothrow_copy_constructible_v<D>)
			requires(std::copy_constructible<D> && !non_const_ref<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter.
		 *
		 * A handle constructed like this is guaranteed to hold a value.
		 *
		 * Requires that @em deleter is move-constructible.
		 *
		 * @remark If @em E is an expected input, handle(T, deleter, no_empty_handle_check_t) may be used instead.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * @pre @em value is not allowed to equal @em E.
		 * @endparblock
		 * @param[in] deleter The deleter value to move.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, D&& deleter) noexcept(std::is_nothrow_move_constructible_v<D>)
			requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value without checking for the invalid @em E case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Requires that @em deleter is default constructible (pointers are not counted as default constructible).
		 *
		 * @remark handle(T) should be used in cases where this isn't necessary.
		 *
		 * @param[in] value The base type value that will be managed by the handle.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, no_empty_handle_check_t) noexcept
			requires(default_constructible_handle_deleter<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em E
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Requires that @em deleter is copy-constructible.
		 *
		 * @remark handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * @param[in] value The base type value that will be managed by the handle.
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, D& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_copy_constructible_v<D>)
			requires(std::copy_constructible<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em E
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Requires that @em deleter is copy-constructible.
		 *
		 * @remark handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * @param[in] value The base type value that will be managed by the handle.
		 * @param[in] deleter The deleter value to copy.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, const D& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_copy_constructible_v<D>)
			requires(std::copy_constructible<D> && !non_const_ref<D>);

		/**************************************************************************************************************
		 * Constructs a handle from a base type value and a deleter without checking for the invalid @em E
		 * case.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * Requires that @em deleter is move-constructible.
		 *
		 * @remark handle(T, deleter) should be used in cases where this isn't necessary.
		 *
		 * @param[in] value The base type value that will be managed by the handle.
		 * @param[in] deleter The deleter value to move.
		 **************************************************************************************************************/
		constexpr explicit handle(T value, D&& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_move_constructible_v<D>)
			requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>);

		/**************************************************************************************************************
		 * Constructs a handle by moving from another handle.
		 *
		 * A handle constructed like this may or may not hold a value.
		 *
		 * @param[in] move
		 * @parblock
		 * The handle whose ownership over a value will be moved into the new handle.
		 *
		 * It will be left in a valid empty state afterwards.
		 * @endparblock
		 **************************************************************************************************************/
		template <handle_deleter<T> D1> constexpr handle(handle<T, E, D1>&& move) noexcept;

		/**************************************************************************************************************
		 * Destroys the handle.
		 *
		 * If the handle is not empty, @em deleter will be called with the contained value.
		 **************************************************************************************************************/
		constexpr ~handle() noexcept;

		/**************************************************************************************************************
		 * Move-assigns the handle.
		 *
		 * If any value was contained in the handle before this, @em deleter is called with said value.
		 *
		 * @param[in] r
		 * @parblock
		 * The handle whose ownership over a value will be moved into the handle.
		 *
		 * It will be left in a valid empty state afterwards.
		 * @endparblock
		 *
		 * @return A reference to assigned-to handle.
		 **************************************************************************************************************/
		constexpr handle& operator=(handle&& r) noexcept;

		/**************************************************************************************************************
		 * Checks if the handle contains a value.
		 *
		 * @return true if the handle is not empty.
		 **************************************************************************************************************/
		constexpr bool has_value() const noexcept;

		/**************************************************************************************************************
		 * Checks if the handle contains a value, see has_value().
		 **************************************************************************************************************/
		constexpr explicit operator bool() const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's base type value.
		 *
		 * @pre This function cannot be called if the handle is empty.
		 *
		 * @remark get(no_empty_handle_check_t) may be used in cases where the empty value is an expected input.
		 *
		 * @return A reference to the handle's base type value.
		 **************************************************************************************************************/
		constexpr const T& get() const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's base type value.
		 *
		 * This function circuments the notion of an "empty" handle and returns a value even if it's @em E.
		 *
		 * @remark get() should be used in cases where this isn't necessary.
		 *
		 * @return A reference to the handle's base type value.
		 **************************************************************************************************************/
		constexpr const T& get(no_empty_handle_check_t) const noexcept;

		/**************************************************************************************************************
		 * Gets the handle's deleter.
		 *
		 * @return A mutable reference to the handle's deleter.
		 **************************************************************************************************************/
		constexpr D& get_deleter() noexcept;

		/**************************************************************************************************************
		 * Gets the handle's deleter.
		 *
		 * @return An immutable reference to the handle's deleter.
		 **************************************************************************************************************/
		constexpr D& get_deleter() const noexcept;

		/**************************************************************************************************************
		 * Releases ownership over the handle, if any.
		 *
		 * The handle will be empty after this call.
		 *
		 * @return The value previously held by the handle.
		 **************************************************************************************************************/
		constexpr T release() noexcept;

		/**************************************************************************************************************
		 * Resets the handle to an empty state.
		 *
		 * If the handle is not empty before this call, @em deleter will be called with the contained value.
		 **************************************************************************************************************/
		constexpr void reset() noexcept;

		/**************************************************************************************************************
		 * Resets the handle to a non-empty state.
		 *
		 * If the handle is not empty before this call, @em deleter will be called with the contained value.
		 *
		 * @remark reset(T, no_empty_handle_check_t) may be used in cases where the empty value is an expected input.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * May not equal @em E, an assertion may be raised otherwise.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr void reset(T value) noexcept;

		/**************************************************************************************************************
		 * Resets the handle to a new state.
		 *
		 * If the handle is not empty before this call, @em deleter will be called with the contained value.
		 *
		 * @remark reset(T) should be preferred in most cases.
		 *
		 * @param[in] value
		 * @parblock
		 * The base type value that will be managed by the handle.
		 *
		 * If it equals @em E, the handle will be empty afterwards, otherwise it will contain a value.
		 * @endparblock
		 **************************************************************************************************************/
		constexpr void reset(T value, no_empty_handle_check_t) noexcept;

		/**************************************************************************************************************
		 * Swaps the ownership over values between two handles.
		 *
		 * Either handle may or may not be empty, in which case the swapped handle will take on that state.
		 *
		 * @param other The handle to swap values with.
		 **************************************************************************************************************/
		constexpr void swap(handle& other) noexcept;

	  private:
		T _base;
	};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::handleable T, T E, tr::handle_deleter<T> D> struct std::hash<tr::handle<T, E, D>> {
	constexpr auto operator()(const tr::handle<T, E, D>& handle) const noexcept;
};

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr auto std::hash<tr::handle<T, E, D>>::operator()(const tr::handle<T, E, D>& handle) const noexcept
{
	return std::hash<T>{}(handle.get(tr::NO_EMPTY_HANDLE_CHECK));
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle() noexcept
	requires(default_constructible_handle_deleter<D>)
	: _base{E}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value) noexcept
	requires(default_constructible_handle_deleter<D>)
	: _base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter) noexcept(std::is_nothrow_copy_constructible_v<D>)
	requires(std::copy_constructible<D>)
	: D{deleter}, _base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter) noexcept(std::is_nothrow_copy_constructible_v<D>)
	requires(std::copy_constructible<D> && !non_const_ref<D>)
	: D{deleter}, _base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter) noexcept(std::is_nothrow_move_constructible_v<D>)
	requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>)
	: D{std::move(deleter)}, _base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, no_empty_handle_check_t) noexcept
	requires(default_constructible_handle_deleter<D>)
	: _base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_copy_constructible_v<D>)
	requires(std::copy_constructible<D>)
	: D{deleter}, _base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_copy_constructible_v<D>)
	requires(std::copy_constructible<D> && !non_const_ref<D>)
	: D{deleter}, _base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter, no_empty_handle_check_t) noexcept(std::is_nothrow_move_constructible_v<D>)
	requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>)
	: D{std::move(deleter)}, _base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
template <tr::handle_deleter<T> D1>
constexpr tr::handle<T, E, D>::handle(handle<T, E, D1>&& move) noexcept
	: _base{std::exchange(move._base, E)}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::~handle<T, E, D>() noexcept
{
	if (_base != E) {
		D::operator()(_base);
	}
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>& tr::handle<T, E, D>::operator=(handle&& r) noexcept
{
	std::ignore = handle{std::move(*this)};
	std::swap(_base, r._base);
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr bool tr::handle<T, E, D>::has_value() const noexcept
{
	return _base != E;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::operator bool() const noexcept
{
	return has_value();
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr const T& tr::handle<T, E, D>::get() const noexcept
{
	TR_ASSERT(_base != E, "Cannot get the value of an empty handle.");

	return _base;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr const T& tr::handle<T, E, D>::get(no_empty_handle_check_t) const noexcept
{
	return _base;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter() noexcept
{
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter() const noexcept
{
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr T tr::handle<T, E, D>::release() noexcept
{
	return std::exchange(_base, E);
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset() noexcept
{
	std::ignore = std::exchange(*this, handle{});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset(T value) noexcept
{
	std::ignore = std::exchange(*this, handle{value});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr void tr::handle<T, E, D>::reset(T value, no_empty_handle_check_t) noexcept
{
	std::ignore = std::exchange(*this, handle{value, NO_EMPTY_HANDLE_CHECK});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::swap(handle& other) noexcept
{
	std::swap(_base, other._base);
}

/// @endcond
