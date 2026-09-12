/// @file
/// @brief Provides `tr::opt_ref`.

#pragma once
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// Wrapper over a pointer representing optional reference semantics.
	/// @tparam T Type being referenced.
	template <typename T>
	class opt_ref
	{
	  public:
		/// @name Constructors
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
		[[nodiscard]] constexpr opt_ref(T& ref) noexcept
			: m_base{std::addressof(ref)}
		{
		}

		/// Wraps a pointer.
		/// @param ptr Pointer to wrap.
		[[nodiscard]] constexpr explicit opt_ref(T* ptr) noexcept
			: m_base{ptr}
		{
		}

		/// Wrapping an rvalue is not permitted.
		constexpr opt_ref(T&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Const-qualifies the optional reference.
		/// @return Const-qualified version of the optional reference.
		[[nodiscard]] constexpr operator opt_ref<const T>() const noexcept
		{
			return opt_ref<const T>{m_base};
		}

		/// Converts the reference to a base class reference.
		/// @return Optional reference to the base class.
		template <typename U>
			requires(std::convertible_to<T&, U&>)
		[[nodiscard]] constexpr operator opt_ref<U>() const noexcept
		{
			return opt_ref<U>{static_cast<U*>(as_ptr())};
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
		[[nodiscard]] constexpr bool has_ref() const noexcept
		{
			return m_base != nullptr;
		}

		/// @}
		/// @name Access
		/// @{

		/// Converts the optional reference into a pointer to the referenced object, or nullptr.
		/// @return Pointer equivalent to the optional reference.
		[[nodiscard]] constexpr T* as_ptr() const noexcept
		{
			return m_base;
		}

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr T* operator->() const noexcept
		{
			TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

			return m_base;
		}

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr T& operator*() const noexcept
		{
			TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

			return *m_base;
		}

		/// @}

	  private:
		/// Pointer to the referenced object, or nullptr.
		T* m_base{nullptr};
	};

	/// @name Factories
	/// @{

	/// Converts a pointer into an optional reference.
	/// @tparam T Type being referenced.
	/// @param ptr Pointer to wrap into an optional reference.
	/// @return Equivalent optional reference.
	template <typename T>
	[[nodiscard]] constexpr opt_ref<T> make_opt_ref(T* ptr) noexcept
	{
		return opt_ref<T>{ptr};
	}

	/// @}
	/// @name Overloaded operators
	/// @{

	/// Compares whether a reference and an optional reference point to the same object.
	/// @tparam T Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename T>
	[[nodiscard]] constexpr bool operator==(opt_ref<T> lhs, const std::type_identity_t<T>& rhs) noexcept
	{
		return lhs.as_ptr() == &rhs;
	}

	/// @}
} // namespace tr