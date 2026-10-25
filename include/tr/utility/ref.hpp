/// @file
/// @brief Provides `tr::ref`.

#pragma once

//

namespace tr
{
	/// Alternative reference wrapper.
	/// @tparam Value Type being referenced.
	template <typename Value>
	class ref
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr ref(Value& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// Wrapping an rvalue is not permitted.
		constexpr ref(Value&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Unwraps the reference.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr operator Value&() const noexcept
		{
			return *m_base;
		}

		/// Const-qualifies the reference.
		/// @return Const-qualified version of the reference.
		[[nodiscard]] constexpr operator ref<const Value>() const noexcept
		{
			return ref<const Value>{*m_base};
		}

		/// Converts the reference to a base class reference.
		/// @tparam Base Base class of `Value`.
		/// @return Reference to the base class.
		template <typename Base>
			requires std::convertible_to<Value&, Base&>
		[[nodiscard]] constexpr operator ref<Base>() const noexcept
		{
			return static_cast<Base&>(**this);
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares whether two references point to the same object.
		/// @param lhs, rhs References to compare.
		/// @return Whether the references point to the same object.
		[[nodiscard]] constexpr friend bool operator==(ref lhs, ref rhs) noexcept = default;

		/// @}
		/// @name Access
		/// @{

		/// Converts the reference into a pointer to the referenced object.
		/// @return Pointer equivalent to the reference.
		[[nodiscard]] constexpr Value* as_ptr() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr Value* operator->() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr Value& operator*() const noexcept
		{
			return *m_base;
		}

		/// @}

	  private:
		/// Pointer to the referenced object.
		Value* m_base;
	};

	/// @name Overloaded operators
	/// @{

	/// Compares whether two references point to the same object.
	/// @tparam Value Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename Value>
	[[nodiscard]] constexpr bool operator==(const ref<Value>& lhs, const Value& rhs) noexcept
	{
		return lhs == ref{rhs};
	}

	/// @}
} // namespace tr