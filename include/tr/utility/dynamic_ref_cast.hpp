/// @file
/// @brief Provides `tr::dynamic_ref_cast()`.

#pragma once
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/ref.hpp>

//

namespace tr
{
	/// @name Casts
	/// @{

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(From& ref) noexcept
	{
		return make_opt_ref(dynamic_cast<To*>(std::addressof(ref)));
	}

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(ref<From> ref) noexcept
	{
		return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
	}

	/// Performs a dynamic_cast on a reference that returns an optional reference.
	/// @tparam To Type to cast to.
	/// @tparam From Type to cast from. `To` must be derived from `From`.
	/// @param ref Optional reference to cast.
	/// @return Reference casted to `To`, or `nullopt` if the cast was not successful or `ref` was empty.
	template <typename To, typename From>
		requires(std::derived_from<To, From>)
	[[nodiscard]] constexpr tr::opt_ref<To> dynamic_ref_cast(opt_ref<From> ref) noexcept
	{
		return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
	}

	/// @}
} // namespace tr