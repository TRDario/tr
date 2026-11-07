/// @file
/// @brief Provides `tr::ref`.

/// @ingroup wrappers
/// @defgroup ref Reference wrapper
/// @brief Contains `tr::ref` and related functionality.

#pragma once

//

namespace tr
{
	/// @ingroup ref
	/// @brief `std::reference_wrapper` alternative.
	/// @details `tr::ref` implements a reference wrapper similar to `std::reference_wrapper` with a more ergonomic interface, allowing
	/// dereferencing through the reference without needing to call `.get()` to unwrap it.
	/// @tparam Value Type being referenced.
	template <typename Value>
	class ref
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr ref(Value& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// @brief Creates a reference from a reference to a similar type.
		/// @tparam ValueLike Type whose reference is convertible to `Value`.
		/// @param ref Reference to convert.
		template <typename ValueLike>
			requires std::convertible_to<ValueLike&, Value&>
		[[nodiscard]] constexpr ref(ref<ValueLike> ref) noexcept
			: m_base{ref.as_ptr()}
		{
		}

		/// @brief Wrapping an rvalue is not permitted.
		constexpr ref(Value&&) = delete;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Converts the reference into a pointer to the referenced object.
		/// @return Pointer equivalent to the reference.
		[[nodiscard]] constexpr Value* as_ptr() const noexcept
		{
			return m_base;
		}

		/// @brief Unwraps the reference.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr operator Value&() const noexcept
		{
			return *m_base;
		}

		/// @brief Dereferences the referenced object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr Value* operator->() const noexcept
		{
			return m_base;
		}

		/// @brief Dereferences the referenced object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr Value& operator*() const noexcept
		{
			return *m_base;
		}

		/// @}

	  private:
		/// @brief Pointer to the referenced object.
		Value* m_base;
	};

	//

	/// @ingroup ref
	/// @brief Checks whether two references point to the same object.
	/// @details This is a shallow comparison of the addresses of the references, @em not a value comparison.
	/// @tparam Value Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return `true` if the references point to the same object, `false` otherwise.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(ref<Value> lhs, std::type_identity_t<ref<Value>> rhs) noexcept
	{
		return lhs.as_ptr() == rhs.as_ptr();
	}

	/// @ingroup ref
	/// @brief Checks whether two references point to the same object.
	/// @details This is a shallow comparison of the addresses of the references, @em not a value comparison.
	/// @tparam Value Referenced value type.
	/// @param lhs, rhs References to compare.
	/// @return `true` if the references point to the same object, `false` otherwise.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(ref<Value> lhs, const std::type_identity_t<Value>& rhs) noexcept
	{
		return lhs.as_ptr() == std::addressof(rhs);
	}
} // namespace tr