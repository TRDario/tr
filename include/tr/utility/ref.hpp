/// @file
/// @brief Provides `tr::ref`.

#pragma once

//

namespace tr
{
	/// Alternative reference wrapper.
	/// @tparam T Type being referenced.
	template <typename T>
	class ref
	{
	  public:
		/// @name Constructors
		/// @{

		/// Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr ref(T& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// Wrapping an rvalue is not permitted.
		constexpr ref(T&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Unwraps the reference.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr operator T&() const noexcept
		{
			return *m_base;
		}

		/// Const-qualifies the reference.
		/// @return Const-qualified version of the reference.
		[[nodiscard]] constexpr operator ref<const T>() const noexcept
		{
			return ref<const T>{*m_base};
		}

		/// Converts the reference to a base class reference.
		/// @tparam U Base class of `T`.
		/// @return Reference to the base class.
		template <typename U>
			requires(std::convertible_to<T&, U&>)
		[[nodiscard]] constexpr operator ref<U>() const noexcept
		{
			return static_cast<U&>(**this);
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
		[[nodiscard]] constexpr T* as_ptr() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr T* operator->() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr T& operator*() const noexcept
		{
			return *m_base;
		}

		/// @}

	  private:
		/// Pointer to the referenced object.
		T* m_base;
	};

	/// @name Overloaded operators
	/// @{

	/// Compares whether two references point to the same object.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename T>
	[[nodiscard]] constexpr bool operator==(const ref<T>& lhs, const T& rhs) noexcept
	{
		return lhs == ref{rhs};
	}

	/// @}
} // namespace tr