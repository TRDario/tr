/// @file
/// @brief Provides utilities for determining whether a type is a specialization of a template.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
#ifdef TR_DOXYGEN
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking type parameters.
	template <typename Target, template <typename...> typename Template>
	struct is_specialization_of
	{
		/// Holds whether `Target` is a specialization of `Template`.
		static constexpr bool value;
	};
#else
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking type parameters.
	/// @details This is the implementation for the negative case.
	template <typename Target, template <typename...> typename Template>
	struct is_specialization_of : std::false_type
	{
	};

	/// Type trait checking whether a type is a specialization of `Template`.
	/// @tparam Template Template taking type parameters.
	/// @details This is the implementation for the positive case.
	template <typename... Types, template <typename...> typename Template>
	struct is_specialization_of<Template<Types...>, Template> : std::true_type
	{
	};
#endif

	/// Checks whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking type parameters.
	template <typename Target, template <typename...> typename Template>
	inline constexpr bool is_specialization_of_v{is_specialization_of<Target, Template>::value};

	/// Concept denoting a specialization of a template.
	/// @tparam Template Template taking type parameters.
	template <typename Target, template <typename...> typename Template>
	concept specialization_of = is_specialization_of_v<Target, Template>;

	/// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template.
	/// @tparam Template Template taking type parameters.
	template <typename Target, template <typename...> typename Template>
	concept cvref_specialization_of = specialization_of<std::remove_cvref_t<Target>, Template>;

	//

#ifdef TR_DOXYGEN
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking constant parameters.
	template <typename Target, template <auto...> typename Template>
	struct is_specialization_of_c
	{
		/// Holds whether `Target` is a specialization of `Template`.
		static constexpr bool value;
	};
#else
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking constant parameters.
	/// @details This is the implementation for the negative case.
	template <typename Target, template <auto...> typename Template>
	struct is_specialization_of_c : std::false_type
	{
	};

	/// Type trait checking whether a type is a specialization of `Template`.
	/// @tparam Template Template taking constant parameters.
	/// @details This is the implementation for the positive case.
	template <auto... Constants, template <auto...> typename Template>
	struct is_specialization_of_c<Template<Constants...>, Template> : std::true_type
	{
	};
#endif

	/// Checks whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking constant parameters.
	template <typename Target, template <auto...> typename Template>
	inline constexpr bool is_specialization_of_c_v{is_specialization_of_c<Target, Template>::value};

	/// Concept denoting a specialization of a template.
	/// @tparam Template Template taking constant parameters.
	template <typename Target, template <auto...> typename Template>
	concept specialization_of_c = is_specialization_of_c_v<Target, Template>;

	/// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template.
	/// @tparam Template Template taking constant parameters.
	template <typename Target, template <auto...> typename Template>
	concept cvref_specialization_of_c = specialization_of_c<std::remove_cvref_t<Target>, Template>;

	//

#ifdef TR_DOXYGEN
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a constant and type parameters.
	template <typename Target, template <auto, typename...> typename Template>
	struct is_specialization_of_ct
	{
		/// Holds whether `Target` is a specialization of `Template`.
		static constexpr bool value;
	};
#else
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a constant and type parameters.
	/// @details This is the implementation for the negative case.
	template <typename Target, template <auto, typename...> typename Template>
	struct is_specialization_of_ct : std::false_type
	{
	};

	/// Type trait checking whether a type is a specialization of `Template`.
	/// @tparam Template Template taking a constant and type parameters.
	/// @details This is the implementation for the positive case.
	template <auto Constant, typename... Types, template <auto, typename...> typename Template>
	struct is_specialization_of_ct<Template<Constant, Types...>, Template> : std::true_type
	{
	};
#endif

	/// Checks whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a constant and type parameters.
	template <typename Target, template <auto, typename...> typename Template>
	inline constexpr bool is_specialization_of_ct_v{is_specialization_of_ct<Target, Template>::value};

	/// Concept denoting a specialization of a template.
	/// @tparam Template Template taking a constant and type parameters.
	template <typename Target, template <auto, typename...> typename Template>
	concept specialization_of_ct = is_specialization_of_ct_v<Target, Template>;

	/// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template.
	/// @tparam Template Template taking a constant and type parameters.
	template <typename Target, template <auto, typename...> typename Template>
	concept cvref_specialization_of_ct = specialization_of_ct<std::remove_cvref_t<Target>, Template>;

	//

#ifdef TR_DOXYGEN
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a type and constant parameters.
	template <typename Target, template <typename, auto...> typename Template>
	struct is_specialization_of_tc
	{
		/// Holds whether `Target` is a specialization of `Template`.
		static constexpr bool value;
	};
#else
	/// Type trait checking whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a type and constant parameters.
	/// @details This is the implementation for the negative case.
	template <typename Target, template <typename, auto...> typename Template>
	struct is_specialization_of_tc : std::false_type
	{
	};

	/// Type trait checking whether a type is a specialization of `Template`.
	/// @tparam Template Template taking a type and constant parameters.
	/// @details This is the implementation for the positive case.
	template <typename Type, auto... Constants, template <typename, auto...> typename Template>
	struct is_specialization_of_tc<Template<Type, Constants...>, Template> : std::true_type
	{
	};
#endif

	/// Checks whether `Target` is a specialization of `Template`.
	/// @tparam Template Template taking a type and constant parameters.
	template <typename Target, template <typename, auto...> typename Template>
	inline constexpr bool is_specialization_of_tc_v{is_specialization_of_tc<Target, Template>::value};

	/// Concept denoting a specialization of a template.
	/// @tparam Template Template taking a type and constant parameters.
	template <typename Target, template <typename, auto...> typename Template>
	concept specialization_of_tc = is_specialization_of_tc_v<Target, Template>;

	/// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template.
	/// @tparam Template Template taking a type and constant parameters.
	template <typename Target, template <typename, auto...> typename Template>
	concept cvref_specialization_of_tc = specialization_of_tc<std::remove_cvref_t<Target>, Template>;
} // namespace tr