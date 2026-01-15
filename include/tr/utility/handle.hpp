#pragma once
#include "concepts.hpp"
#include "macro.hpp"

namespace tr {
	// Concept that denotes a valid handle base type.
	template <class T>
	concept handleable = std::is_trivially_copyable_v<T> && std::equality_comparable<T>;
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
	template <handleable T, T E, handle_deleter<T> D> class handle : private D {
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
			requires(std::copy_constructible<D> && !non_const_ref<D>);
		// Constructs a handle from a base type value and a deleter.
		constexpr explicit handle(T value, D&& deleter)
			requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>);
		// Constructs a handle from a base type value without checking for the invalid E case.
		constexpr explicit handle(T value, no_empty_handle_check_t)
			requires(default_constructible_handle_deleter<D>);
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, D& deleter, no_empty_handle_check_t)
			requires(std::copy_constructible<D>);
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, const D& deleter, no_empty_handle_check_t)
			requires(std::copy_constructible<D> && !non_const_ref<D>);
		// Constructs a handle from a base type value and a deleter without checking for the invalid E case.
		constexpr explicit handle(T value, D&& deleter, no_empty_handle_check_t)
			requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>);
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
		T m_base;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::handleable T, T E, tr::handle_deleter<T> D> struct std::hash<tr::handle<T, E, D>> {
	constexpr auto operator()(const tr::handle<T, E, D>& handle) const
	{
		return std::hash<T>{}(handle.get(tr::no_empty_handle_check));
	}
};

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle()
	requires(default_constructible_handle_deleter<D>)
	: m_base{E}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value)
	requires(default_constructible_handle_deleter<D>)
	: m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter)
	requires(std::copy_constructible<D>)
	: D{deleter}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter)
	requires(std::copy_constructible<D> && !non_const_ref<D>)
	: D{deleter}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter)
	requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>)
	: D{std::move(deleter)}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, no_empty_handle_check_t)
	requires(default_constructible_handle_deleter<D>)
	: m_base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<D>)
	: D{deleter}, m_base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<D> && !non_const_ref<D>)
	: D{deleter}, m_base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter, no_empty_handle_check_t)
	requires(std::move_constructible<D> && !std::is_lvalue_reference_v<D>)
	: D{std::move(deleter)}, m_base{value}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D>
template <tr::handle_deleter<T> D1>
constexpr tr::handle<T, E, D>::handle(handle<T, E, D1>&& move) noexcept
	: m_base{std::exchange(move.m_base, E)}
{
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::~handle<T, E, D>()
{
	if (m_base != E) {
		D::operator()(m_base);
	}
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>& tr::handle<T, E, D>::operator=(handle&& r) noexcept
{
	std::ignore = handle{std::move(*this)};
	std::swap(m_base, r.m_base);
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr bool tr::handle<T, E, D>::has_value() const
{
	return m_base != E;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::operator bool() const
{
	return has_value();
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr const T& tr::handle<T, E, D>::get() const
{
	TR_ASSERT(m_base != E, "Cannot get the value of an empty handle.");

	return m_base;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr const T& tr::handle<T, E, D>::get(no_empty_handle_check_t) const
{
	return m_base;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter()
{
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter() const
{
	return *this;
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr T tr::handle<T, E, D>::release()
{
	return std::exchange(m_base, E);
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset()
{
	std::ignore = std::exchange(*this, handle{});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset(T value)
{
	std::ignore = std::exchange(*this, handle{value});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset(T value, no_empty_handle_check_t)
{
	std::ignore = std::exchange(*this, handle{value, no_empty_handle_check});
}

template <tr::handleable T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::swap(handle& other)
{
	std::swap(m_base, other.m_base);
}