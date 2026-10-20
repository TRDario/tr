/// @file
/// @brief Provides `tr::copy_qualifiers_t`.

#pragma once

//

namespace tr
{
	/// Type trait copying const/ref qualifiers from `Qualified` to `T`.
	/// @tparam Qualified Type holding the desired const/ref qualifiers.
	template <typename T, typename Qualified>
	struct copy_qualifiers
	{
	  private:
		/// Adds const to `T` if `Qualified` is const.
		using maybe_const = std::conditional_t<std::is_const_v<std::remove_reference_t<Qualified>>, std::add_const_t<T>, T>;

		/// Adds & to `T` if `Qualified` is &.
		using maybe_lref = std::conditional_t<std::is_lvalue_reference_v<Qualified>, std::add_lvalue_reference_t<maybe_const>, maybe_const>;

	  public:
		/// Holds `T` qualified with the same qualifiers as `Qualified`.
		/// @hideinitializer
		using type = std::conditional_t<std::is_rvalue_reference_v<Qualified>, std::add_rvalue_reference_t<maybe_lref>, maybe_lref>;
	};

	/// Copies const/ref qualifiers from `Qualified` to `T`.
	/// @tparam Qualified Type holding the desired const/ref qualifiers.
	template <typename T, typename Qualified>
	using copy_qualifiers_t = copy_qualifiers<T, Qualified>::type;
} // namespace tr