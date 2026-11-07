/// @file
/// @brief Provides `tr::opt_ref` and related functionality.

/// @ingroup wrappers
/// @defgroup opt_ref Optional reference wrapper
/// @brief Contains `tr::opt_ref` and related functionality.

#pragma once
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// @ingroup opt_ref
	/// @brief Optional reference to an object.
	/// @details `tr::opt_ref` implements an interface similar to `std::optional` for optional references. It is fundamentally a wrapper
	/// around a raw pointer.
	/// @tparam Value Type being referenced.
	template <typename Value>
	class opt_ref
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Creates an empty optional reference.
		[[nodiscard]] constexpr opt_ref() noexcept = default;

		/// @brief Creates an empty optional reference.
		[[nodiscard]] constexpr opt_ref(std::nullopt_t) noexcept
			: m_base{nullptr}
		{
		}

		/// @brief Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr opt_ref(Value& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// @brief Wrapping an rvalue is not permitted.
		constexpr opt_ref(Value&&) = delete;

		/// @brief Wraps a pointer.
		/// @param ptr Pointer to wrap.
		[[nodiscard]] constexpr explicit opt_ref(Value* ptr) noexcept
			: m_base{ptr}
		{
		}

		/// @brief Creates an optional reference from an optional reference to a similar type.
		/// @tparam ValueLike Type whose reference is convertible to `Value`.
		/// @param ref Optional reference to convert.
		template <typename ValueLike>
			requires std::convertible_to<ValueLike&, Value&>
		[[nodiscard]] constexpr opt_ref(opt_ref<ValueLike> ref) noexcept
			: m_base{ref.as_ptr()}
		{
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Converts the optional reference into a pointer to the referenced object, or nullptr.
		/// @return Pointer equivalent to the optional reference.
		[[nodiscard]] constexpr Value* as_ptr() const noexcept
		{
			return m_base;
		}

		/// @brief Checks whether the optional reference holds a reference to an object.
		/// @return `true` if a reference is contained, `false` otherwise.
		[[nodiscard]] explicit constexpr operator bool() const noexcept
		{
			return m_base;
		}

		/// @brief Checks whether the optional reference holds a reference to an object.
		/// @return `true` if a reference is contained, `false` otherwise.
		[[nodiscard]] constexpr bool has_value() const noexcept
		{
			return m_base;
		}

		/// @brief Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr Value* operator->() const noexcept
		{
			TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

			return m_base;
		}

		/// @brief Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr Value& operator*() const noexcept
		{
			TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

			return *m_base;
		}

		/// @}

	  private:
		/// @brief Pointer to the referenced object, or nullptr.
		Value* m_base{nullptr};
	};

	//

	/// @ingroup opt_ref
	/// @brief Converts a pointer into an optional reference.
	/// @tparam Value Type being referenced.
	/// @param ptr Pointer to wrap into an optional reference.
	/// @return Equivalent optional reference.
	template <typename Value>
	[[nodiscard]] constexpr opt_ref<Value> make_opt_ref(Value* ptr) noexcept
	{
		return opt_ref<Value>{ptr};
	}

	//

	/// @ingroup opt_ref
	/// @brief Checks whether two optional references point to the same object (or if both are empty).
	/// @details This is a shallow comparison of the addresses of the references, @em not a value comparison.
	/// @tparam Value Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return `true` if the references point to the same object (or are both empty), `false` otherwise.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(opt_ref<Value> lhs, std::type_identity_t<opt_ref<Value>> rhs) noexcept
	{
		return lhs.as_ptr() == rhs.as_ptr();
	}

	/// @ingroup opt_ref
	/// @brief Checks whether a reference and an optional reference point to the same object.
	/// @details This is a shallow comparison of the addresses of the references, @em not a value comparison.
	/// @tparam Value Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return `true` if the references point to the same object, `false` otherwise.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(opt_ref<Value> lhs, const std::type_identity_t<Value>& rhs) noexcept
	{
		return lhs.as_ptr() == std::addressof(rhs);
	}
} // namespace tr