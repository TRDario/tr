/// @file
/// @brief Provides variant utilities.

/// @ingroup wrappers
/// @defgroup variant Variants
/// @brief Contains variant wrappers.

#pragma once
#include <tr/utility/copy_qualifiers.hpp>
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/pack_traits.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @ingroup variant
	/// @brief Concept checking for a visitor applicable to a single variant.
	/// @tparam Target Type to check for being a visitor.
	/// @tparam Alternatives Qualified alternatives of the variant.
	template <typename Target, typename... Alternatives>
	concept variant_visitor = (std::invocable<Target, Alternatives> && ...) && all_same<std::invoke_result_t<Target, Alternatives>...>;

	/// @ingroup variant
	/// @brief Concept checking for a non-throwing visitor applicable to a single variant.
	/// @tparam Target Type to check for being a nothrow visitor.
	/// @tparam Alternatives Qualified alternatives of the variant.
	template <typename Target, typename... Alternatives>
	concept nothrow_variant_visitor = variant_visitor<Target, Alternatives...> &&
									  (std::is_nothrow_invocable_v<Target, Alternatives> && ...);

	//

	/// @ingroup variant
	/// @brief Expanded standard variant type.
	/// @details Aside from supporting the regular variant interface, `tr::variant` is convertible to an optional reference and has the
	/// getter/visitor functions as methods.
	/// @tparam Alternatives Variant alternatives.
	template <typename... Alternatives>
		requires(sizeof...(Alternatives) >= 1)
	class variant : public std::variant<Alternatives...>
	{
	  public:
		/// @brief Member version of `std::variant_size_v`.
		static constexpr usize variant_size{sizeof...(Alternatives)};

		/// @brief Member version of `std::variant_alternative_t`.
		/// @tparam I Index of the alternative.
		template <usize I>
		using alternative_t = std::variant_alternative_t<I, std::variant<Alternatives...>>;

		/// @brief Casts a derived type back to a qualified `tr::variant`.
		/// @tparam Self Derived variant type.
		template <typename Self>
		using base_t = copy_qualifiers_t<variant, Self>;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		using std::variant<Alternatives...>::variant;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Attempts to return a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr operator opt_ref<Alternative>() noexcept
		{
			return get_if<Alternative>();
		}

		/// @brief Attempts to return a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr operator opt_ref<const Alternative>() const noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// @brief Attempts to return a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr opt_ref<Alternative> get_if() noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// @brief Attempts to return a specific alternative stored in the variant.
		/// @tparam Alternative Alternative to get.
		/// @return Alternative stored in the variant, or an empty optional if it is not held.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr opt_ref<const Alternative> get_if() const noexcept
		{
			return make_opt_ref(std::get_if<Alternative>(this));
		}

		/// @brief Checks whether a specific alternative is held in the variant.
		/// @tparam Alternative Alternative to check for.
		/// @return `true` if the variant is holding `Alternative`, `false` otherwise.
		template <one_of<Alternatives...> Alternative>
		[[nodiscard]] constexpr bool is() const noexcept
		{
			return std::holds_alternative<Alternative>(*this);
		}

		/// @brief Returns a specific alternative stored in the variant.
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
		/// @name Visitation
		/// @{

		/// @brief Applies a visitor on the variant.
		/// @tparam Visitor Type of the applied visitor.
		/// @param visitor Visitor to apply.
		/// @return Result of the visit, if any.
		template <typename Self, variant_visitor<copy_qualifiers_t<Alternatives, Self&&>...> Visitor>
		[[nodiscard]] constexpr decltype(auto) visit(this Self&& self, Visitor&& visitor)
			noexcept(nothrow_variant_visitor<Visitor, copy_qualifiers_t<Alternatives, Self&&>...>)
		{
			return std::visit(std::forward<Visitor>(visitor), static_cast<base_t<Self>&&>(self));
		}

		/// @}
	};
} // namespace tr

//

/// @ingroup variant
/// @brief Specialization of `tr::variant_size` for `tr::variant`-like types.
/// @tparam VariantLike Variant-like type with `variant_size` defined as a member constant.
template <typename VariantLike>
	requires requires { VariantLike::variant_size; }
struct std::variant_size<VariantLike> : std::integral_constant<std::size_t, VariantLike::variant_size>
{
};

/// @ingroup variant
/// @brief Specialization of `tr::variant_alternative` for `tr::variant`-like types.
/// @tparam I Index of the alternative to get.
/// @tparam VariantLike Variant-like type with `alternative_t` defined as member type alias.
template <std::size_t I, typename VariantLike>
	requires requires { typename VariantLike::template alternative_t<I>; }
struct std::variant_alternative<I, VariantLike>
{
	/// Holds the type of the alternative at index `I` of the variant.
	using type = VariantLike::template alternative_t<I>;
};