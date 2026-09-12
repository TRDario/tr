/// @file
/// @brief Provides `tr::handle`.

#pragma once
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// Valid handle deleter type for a handle to `Base`.
	template <typename T, typename Base>
	concept handle_deleter = std::invocable<T, Base> && (std::move_constructible<T> || std::copy_constructible<T>) &&
							 std::is_nothrow_invocable_v<T, Base>;

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
		[[nodiscard]] constexpr handle() noexcept(std::is_nothrow_default_constructible_v<Deleter>)
			requires(default_constructible_handle_deleter<Deleter>)
			: m_base{Empty}
		{
		}

		/// Constructs a handle from a base type value.
		/// @param value Value to hold.
		[[nodiscard]] constexpr explicit handle(Base value) noexcept(std::is_nothrow_default_constructible_v<Deleter>)
			requires(default_constructible_handle_deleter<Deleter>)
			: m_base{value}
		{
			TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
		}

		/// Constructs a handle from a base type value without checking for the invalid case.
		/// @param value Value to hold.
		[[nodiscard]] constexpr explicit handle(Base value, maybe_empty_t) noexcept(std::is_nothrow_default_constructible_v<Deleter>)
			requires(default_constructible_handle_deleter<Deleter>)
			: m_base{value}
		{
		}

		/// Default-constructs an empty handle.
		/// @param deleter Deleter instance.
		[[nodiscard]] constexpr handle(Deleter&& deleter) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			: Deleter{std::forward<Deleter>(deleter)}
			, m_base{Empty}
		{
		}

		/// Constructs a handle from a base type value and a deleter.
		/// @pre `value` must not be equal to `Empty`.
		/// @param value Value to hold.
		/// @param deleter Deleter instance.
		[[nodiscard]] constexpr explicit handle(Base value, Deleter&& deleter) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			requires(std::move_constructible<Deleter>)
			: Deleter{std::forward<Deleter>(deleter)}
			, m_base{value}
		{
			TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
		}

		/// Constructs a handle from a base type value and a deleter without checking for the invalid case.
		/// @param value Value to hold.
		/// @param deleter Deleter instance.
		[[nodiscard]] constexpr explicit handle(Base value, Deleter&& deleter, maybe_empty_t)
			noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			requires(std::move_constructible<Deleter>)
			: Deleter{std::forward<Deleter>(deleter)}
			, m_base{value}
		{
		}

		/// Constructs a handle by moving from another handle.
		/// @param rhs Handle to move.
		[[nodiscard]] constexpr handle(handle&& rhs) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
			requires(std::move_constructible<Deleter>)
			: Deleter{std::move(rhs)}
			, m_base{std::exchange(rhs.m_base, Empty)}
		{
		}

		/// Destroys the handle.
		constexpr ~handle() noexcept
		{
			if (m_base != Empty) {
				Deleter::operator()(m_base);
			}
		}

		/// @}
		/// @name Assignment operators
		/// @{

		/// Move-assigns the handle.
		/// @param rhs Handle to move.
		/// @return Reference to `*this`.
		constexpr handle& operator=(handle&& rhs) noexcept(std::is_nothrow_move_assignable_v<Deleter>)
			requires(std::is_move_assignable_v<Deleter>)
		{
			if (has_value()) {
				Deleter::operator()(m_base);
			}
			m_base = std::exchange(rhs.m_base, Empty);
			get_deleter() = std::move(rhs.get_deleter());
			return *this;
		}

		/// @}
		/// @name State
		/// @{

		/// Checks if the handle contains a value.
		/// @return `true` if the handle contains a value, `false` otherwise.
		[[nodiscard]] constexpr bool has_value() const noexcept
		{
			return m_base != Empty;
		}

		/// Checks if the handle contains a value.
		/// @return `true` if the handle contains a value, `false` otherwise.
		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return has_value();
		}

		/// @}
		/// @name Getters
		/// @{

		/// Gets the handle's base type value.
		/// @pre The handle must contain a value.
		/// @return Reference to the contained value.
		[[nodiscard]] constexpr const Base& get() const noexcept
		{
			TR_ASSERT(m_base != Empty, "Cannot get the value of an empty handle.");

			return m_base;
		}

		/// Gets the handle's base type value without checking for the invalid case.
		/// @return Reference to the contained value.
		[[nodiscard]] constexpr const Base& get(maybe_empty_t) const noexcept
		{
			return m_base;
		}

		/// Gets the handle's deleter.
		/// @return Reference to the handle's deleter.
		[[nodiscard]] constexpr Deleter& get_deleter() noexcept
		{
			return *this;
		}

		/// Gets the handle's deleter.
		/// @return Reference to the handle's deleter.
		[[nodiscard]] constexpr const Deleter& get_deleter() const noexcept
		{
			return *this;
		}

		/// @}
		/// @name Resetting
		/// @{

		/// Releases ownership over the handle, if any.
		/// @return Previously contained value.
		[[nodiscard]] constexpr Base release() noexcept
		{
			return std::exchange(m_base, Empty);
		}

		/// Resets the handle to an empty state.
		constexpr void reset() noexcept
		{
			if (has_value()) {
				Deleter::operator()(m_base);
			}
			m_base = Empty;
		}

		/// Resets the handle to a non-empty state.
		/// @pre `value` must not be equal to `Empty`.
		/// @param value New value to hold.
		constexpr void reset(Base value) noexcept
		{
			TR_ASSERT(value != Empty, "Cannot reset a handle to a value set aside as the empty value.");

			if (has_value()) {
				Deleter::operator()(m_base);
			}
			m_base = value;
		}

		/// Resets the handle to a new state without checking for the invalid case.
		/// @param value New value to hold.
		constexpr void reset(Base value, maybe_empty_t) noexcept
		{
			if (has_value()) {
				Deleter::operator()(m_base);
			}
			m_base = value;
		}

		/// @}

	  private:
		/// Wrapped base value.
		Base m_base;
	};
} // namespace tr

//

#ifndef TR_DOXYGEN_SKIP
/// Handle hasher.
/// @tparam Base Wrapped type.
/// @tparam Empty Empty handle sentinel value.
/// @tparam Deleter Deleter invoked when destroying a handled value.
template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
struct boost::hash<tr::handle<Base, Empty, Deleter>>
{
	/// Hashes a handle.
	/// @param handle Handle to hash.
	/// @return Hashed value.
	constexpr auto operator()(const tr::handle<Base, Empty, Deleter>& handle) const noexcept
	{
		return boost::hash<Base>{}(handle.get(tr::maybe_empty));
	}
};
#endif