/// @file
/// @brief Provides variant utilities.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/copy_qualifiers.hpp>
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Concept denoting a visitor applicable to a single variant.
	/// @tparam Alternatives Qualified alternatives of the variant.
	template <typename T, typename... Alternatives>
	concept variant_visitor = (std::invocable<T, Alternatives> && ...) && all_same<std::invoke_result_t<T, Alternatives>...>;

	//

	/// Expanded variant interface.
	/// @tparam Alternatives Variant alternatives.
	template <typename... Alternatives>
		requires(sizeof...(Alternatives) >= 1)
	class variant : public std::variant<Alternatives...>
	{
	  public:
		/// Member version of `std::variant_size_v`.
		static constexpr usize variant_size{sizeof...(Alternatives)};

		/// Member version of `std::variant_alternative_t`.
		/// @tparam I Index of the alternative.
		template <usize I>
		using alternative_t = std::variant_alternative_t<I, std::variant<Alternatives...>>;

		/// Casts a derived type back to a qualified `tr::variant`.
		/// @tparam Self Derived variant type.
		template <typename Self>
		using base_t = decltype(std::forward_like<Self>(std::declval<variant>()));

		/// @name Constructors and destructors
		/// @{

		using std::variant<Alternatives...>::variant;

		/// @}
		/// @name Access
		/// @{

		/// Tries to get a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr operator opt_ref<Alternative>() noexcept
		{
			return get_if<Alternative>();
		}

		/// Tries to get a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr operator opt_ref<const Alternative>() const noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// Tries to get a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr opt_ref<Alternative> get_if() noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// Tries to get a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr opt_ref<const Alternative> get_if() const noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// Gets whether a specific alternative is held in the variant.
		/// @tparam Alternative Alternative to check for.
		/// @return `true` if the variant is holding `Alternative`, `false` otherwise.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr bool is() const noexcept
		{
			return std::holds_alternative<Alternative>(*this);
		}

		/// Gets a specific alternative stored in the variant.
		/// @pre `self` must be storing a value of type `Alternative`.
		/// @tparam Alternative Alternative to get.
		/// @return Reference to the alternative stored in the variant.
		template <one_of<Alternatives...> Alternative, typename Self>
		[[nodiscard]] decltype(auto) get(this Self&& self) noexcept
		{
			TR_ASSERT(static_cast<base_t<Self>&&>(self).template is<Alternative>(),
					  "Tried to access invalid alternative '{}' in a variant.", type_name<Alternative>());

			try {
				return std::get<Alternative>(static_cast<base_t<Self>&&>(self));
			}
			catch (...) {
				TR_UNREACHABLE;
			}
		}

		/// @}
		/// @name Visiting
		/// @{

		/// Applies a visitor on the variant.
		/// @tparam Visitor Type of the applied visitor.
		/// @param visitor Visitor to apply.
		/// @return Result of the visit, if any.
		template <typename Self, variant_visitor<copy_qualifiers_t<Alternatives, Self&&>...> Visitor>
		[[nodiscard]] constexpr decltype(auto) visit(this Self&& self, Visitor&& visitor)
		{
			return std::visit(std::forward<Visitor>(visitor), static_cast<base_t<Self>&&>(self));
		}

		/// @}
	};
} // namespace tr

//

/// Specialization of `tr::variant_size` for `tr::variant`-like types.
/// @tparam VariantLike Variant-like type with `variant_size` defined as a member constant.
template <typename VariantLike>
	requires requires { VariantLike::variant_size; }
struct std::variant_size<VariantLike> : std::integral_constant<std::size_t, VariantLike::variant_size>
{
};

/// Specialization of `tr::variant_alternative` for `tr::variant`-like types.
/// @tparam I Index of the alternative to get.
/// @tparam VariantLike Variant-like type with `alternative_t` defined as member type alias.
template <std::size_t I, typename VariantLike>
	requires requires { typename VariantLike::template alternative_t<I>; }
struct std::variant_alternative<I, VariantLike>
{
	using type = VariantLike::template alternative_t<I>;
};