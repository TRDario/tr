/// @file
/// @brief Provides `tr::opt_ref`.

#pragma once
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// Wrapper over a pointer representing optional reference semantics.
	/// @tparam Value Type being referenced.
	template <typename Value>
	class opt_ref
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty optional reference.
		[[nodiscard]] constexpr opt_ref() noexcept = default;

		/// Creates an empty optional reference.
		[[nodiscard]] constexpr opt_ref(std::nullopt_t) noexcept
			: m_base{nullptr}
		{
		}

		/// Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr opt_ref(Value& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// Wraps a pointer.
		/// @param ptr Pointer to wrap.
		[[nodiscard]] constexpr explicit opt_ref(Value* ptr) noexcept
			: m_base{ptr}
		{
		}

		/// Wrapping an rvalue is not permitted.
		constexpr opt_ref(Value&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Const-qualifies the optional reference.
		/// @return Const-qualified version of the optional reference.
		[[nodiscard]] constexpr operator opt_ref<const Value>() const noexcept
		{
			return opt_ref<const Value>{m_base};
		}

		/// Converts the reference to a base class reference.
		/// @tparam Base Base class of `Value`.
		/// @return Optional reference to the base class.
		template <typename Base>
			requires std::convertible_to<Value&, Base&>
		[[nodiscard]] constexpr operator opt_ref<Base>() const noexcept
		{
			return opt_ref<Base>{static_cast<Base*>(as_ptr())};
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares whether two references point to the same object (or both are empty).
		/// @param lhs, rhs References to compare.
		/// @return Whether the references point to the same object.
		[[nodiscard]] constexpr friend bool operator==(opt_ref lhs, opt_ref rhs) noexcept = default;

		/// @}
		/// @name Status
		/// @{

		/// Returns whether the optional reference holds a reference to an object.
		/// @return `true` if a reference is contained, `false` otherwise.
		[[nodiscard]] constexpr bool has_value() const noexcept
		{
			return m_base != nullptr;
		}

		/// @}
		/// @name Access
		/// @{

		/// Converts the optional reference into a pointer to the referenced object, or nullptr.
		/// @return Pointer equivalent to the optional reference.
		[[nodiscard]] constexpr Value* as_ptr() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr Value* operator->() const noexcept
		{
			TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

			return m_base;
		}

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr Value& operator*() const noexcept
		{
			TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

			return *m_base;
		}

		/// @}

	  private:
		/// Pointer to the referenced object, or nullptr.
		Value* m_base{nullptr};
	};

	/// @name Factories
	/// @{

	/// Converts a pointer into an optional reference.
	/// @tparam Value Type being referenced.
	/// @param ptr Pointer to wrap into an optional reference.
	/// @return Equivalent optional reference.
	template <typename Value>
	[[nodiscard]] constexpr opt_ref<Value> make_opt_ref(Value* ptr) noexcept
	{
		return opt_ref<Value>{ptr};
	}

	/// @}
	/// @name Overloaded operators
	/// @{

	/// Compares whether a reference and an optional reference point to the same object.
	/// @tparam Value Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(opt_ref<Value> lhs, const std::type_identity_t<Value>& rhs) noexcept
	{
		return lhs.as_ptr() == &rhs;
	}

	/// @}
} // namespace tr