/// @file
/// @brief Provides `tr::ref`, `tr::opt_ref`, and related functions.

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
		[[nodiscard]] constexpr ref(T& ref) noexcept;

		/// Wrapping an rvalue is not permitted.
		constexpr ref(T&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Unwraps the reference.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr operator T&() const noexcept;

		/// Const-qualifies the reference.
		/// @return Const-qualified version of the reference.
		[[nodiscard]] constexpr operator ref<const T>() const noexcept;

		/// Converts the reference to a base class reference.
		/// @tparam U Base class of `T`.
		/// @return Reference to the base class.
		template <typename U>
			requires(std::convertible_to<T&, U&>)
		[[nodiscard]] constexpr operator ref<U>() const noexcept;

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
		[[nodiscard]] constexpr T* as_ptr() const noexcept;

		/// Dereferences the referenced object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr T* operator->() const noexcept;

		/// Dereferences the referenced object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr T& operator*() const noexcept;

		/// @}

	  private:
		/// Pointer to the referenced object.
		T* m_base;
	};

	/// Compares whether two references point to the same object.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename T>
	[[nodiscard]] constexpr bool operator==(const ref<T>& lhs, const T& rhs) noexcept;

	//

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
		[[nodiscard]] constexpr opt_ref(std::nullopt_t) noexcept;

		/// Wraps a reference.
		/// @param ref Reference to wrap.
		[[nodiscard]] constexpr opt_ref(T& ref) noexcept;

		/// Wrapping an rvalue is not permitted.
		constexpr opt_ref(T&&) = delete;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Const-qualifies the optional reference.
		/// @return Const-qualified version of the optional reference.
		[[nodiscard]] constexpr operator opt_ref<const T>() const noexcept;

		/// Converts the reference to a base class reference.
		/// @return Optional reference to the base class.
		template <typename U>
			requires(std::convertible_to<T&, U&>)
		[[nodiscard]] constexpr operator opt_ref<U>() const noexcept;

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
		[[nodiscard]] constexpr bool has_ref() const noexcept;

		/// @}
		/// @name Access
		/// @{

		/// Converts the optional reference into a pointer to the referenced object, or nullptr.
		/// @return Pointer equivalent to the optional reference.
		[[nodiscard]] constexpr T* as_ptr() const noexcept;

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw pointer to the object.
		[[nodiscard]] constexpr T* operator->() const noexcept;

		/// Dereferences the referenced object.
		/// @pre The optional reference must reference an object.
		/// @return Raw reference to the object.
		[[nodiscard]] constexpr T& operator*() const noexcept;

		/// @}

	  private:
		/// Pointer to the referenced object, or nullptr.
		T* m_base{nullptr};

		//

		/// Wraps a pointer.
		/// @param ptr Pointer to wrap.
		[[nodiscard]] constexpr explicit opt_ref(T* ptr) noexcept;

		//

		// Uses the private constructor.
		template <typename U>
		friend constexpr opt_ref<U> make_opt_ref(U* ptr) noexcept;
	};

	/// @name Factories
	/// @{

	/// Converts a pointer into an optional reference.
	/// @tparam T Type being referenced.
	/// @param ptr Pointer to wrap into an optional reference.
	/// @return Equivalent optional reference.
	template <typename T>
	[[nodiscard]] constexpr opt_ref<T> make_opt_ref(T* ptr) noexcept;

	/// @}
	/// @name Overloaded operators
	/// @{

	/// Compares whether a reference and an optional reference point to the same object.
	/// @tparam T Type being referenced.
	/// @param lhs, rhs References to compare.
	/// @return Whether the references point to the same object.
	template <typename T>
	[[nodiscard]] constexpr bool operator==(opt_ref<T> lhs, const std::type_identity_t<T>& rhs) noexcept;

	/// @}
	/// @name Casts
	/// @{

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(From& ref) noexcept;

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(ref<From> ref) noexcept;

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Optional reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful or `ref` was empty.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(opt_ref<From> ref) noexcept;

	/// @}
} // namespace tr

#include "impl/reference.hpp" // IWYU pragma: export