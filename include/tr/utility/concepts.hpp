///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides concepts.                                                                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "template.hpp"

namespace tr {
	// Concept denoting an arithmetic type.
	template <class T>
	concept arithmetic = std::is_arithmetic_v<T>;
	// Concept denoting an enumerator type.
	template <class T>
	concept enumerator = std::is_enum_v<T>;

	// Concept denoting an object type (i.e. not a reference).
	template <class T>
	concept object = std::is_object_v<T>;
	// Concept denoting a reference.
	template <class T>
	concept reference = std::is_reference_v<T>;
	// Concept denoting an lvalue reference.
	template <class T>
	concept lvalue_reference = std::is_lvalue_reference_v<T>;
	// Concept denoting an rvalue reference.
	template <class T>
	concept rvalue_reference = std::is_rvalue_reference_v<T>;

	// Concept denoting a const-qualified type.
	template <class T>
	concept const_qualified = std::is_const_v<T>;
	// Concept denoting a volatile-qualified type.
	template <class T>
	concept volatile_qualified = std::is_volatile_v<T>;
	// Concept denoting a cv-unqualified type.
	template <class T>
	concept cv_unqualified = !const_qualified<T> && !volatile_qualified<T>;
	// Concept denoting a cv-unqualified object.
	template <class T>
	concept cv_unqualified_object = object<T> && cv_unqualified<T>;

	// Concept denoting a standard layout type.
	template <class T>
	concept standard_layout = std::is_standard_layout_v<T>;
	// Concept denoting a contiguous range.
	template <class T>
	concept standard_layout_range = std::ranges::contiguous_range<T> && standard_layout<std::ranges::range_value_t<T>>;
	// Concept denoting a sized output range.
	template <class R, class T>
	concept sized_output_range = std::ranges::sized_range<R> && std::output_iterator<std::ranges::iterator_t<R>, T>;
	// Concept denoting a typed contiguous range.
	template <class R, class T>
	concept typed_contiguous_range = std::ranges::contiguous_range<R> && std::same_as<T, std::ranges::range_value_t<R>>;

	// Concept denoting a specialization of a template with type parameters.
	template <class T, template <class...> class Z>
	concept specialization_of = is_specialization_of<T, Z>::value;
	// Concept denoting a specialization of a template with value parameters.
	template <class T, template <auto...> class Z>
	concept specialization_of_v = is_specialization_of_v<T, Z>::value;
	// Concept denoting a specialization of a template with a leading value parameter.
	template <class T, template <auto, class...> class Z>
	concept specialization_of_vt = is_specialization_of_vt<T, Z>::value;
	// Concept denoting a specialization of a template with a leading type and value parameter.
	template <class T, template <class, auto, class...> class Z>
	concept specialization_of_tv = is_specialization_of_tv<T, Z>::value;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with type parameters.
	template <class T, template <class...> class Z>
	concept cvref_specialization_of = specialization_of<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with value parameters.
	template <class T, template <auto...> class Z>
	concept cvref_specialization_of_v = specialization_of_v<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with a leading value parameter.
	template <class T, template <auto, class...> class Z>
	concept cvref_specialization_of_vt = specialization_of_vt<std::remove_cvref_t<T>, Z>;
	// Concept denoting a (potentially cv-qualified or reference to a) specialization of a template with a leading type and value parameter.
	template <class T, template <class, auto, class...> class Z>
	concept cvref_specialization_of_tv = specialization_of_tv<std::remove_cvref_t<T>, Z>;

	// Concept checking if a type is in a list.
	template <class T, class... Ts>
	concept one_of = (std::same_as<T, Ts> || ...);
} // namespace tr