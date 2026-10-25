/// @file
/// @brief Tests utility/hash_concepts.hpp.

#include <tr/utility/hash_concepts.hpp>
#include <tr/utility/hash_map.hpp>

//

/// Tests `tr::hasher` on a positive case.
static_assert(tr::hasher<boost::hash<int>, int>, "boost::hash<int> should fulfill tr::hasher<int>, but does not!");

/// Tests `tr::hasher` on a negative case.
static_assert(!tr::hasher<boost::hash<int>, std::ofstream>, "boost::hash<int> should not fulfill tr::hasher<std::ofstream>, but does!");

/// Tests `tr::hasher` on a negative case.
static_assert(!tr::hasher<int, int>, "int should not fulfill tr::hasher<int>, but does!");

//

/// Tests `tr::equality_predicate` on a lambda.
static_assert(tr::equality_predicate<decltype([](const auto& lhs, const auto& rhs) { return lhs == rhs; }), int>,
			  "Lambda should fulfill tr::equality_predicate<int>, but does not!");

/// Tests `tr::equality_predicate` on `std::equal_to<>`.
static_assert(tr::equality_predicate<std::equal_to<>, int>, "std::equal_to<> should fulfill tr::equality_predicate<int>, but does not!");

/// Tests `tr::equality_predicate` on a negative case.
static_assert(!tr::equality_predicate<int, int>, "int should not fulfill tr::equality_predicate<int>, but does!");

//

/// Tests `tr::hash_keylike` on a positive case.
static_assert(tr::hash_keylike<const char*, std::string, tr::string_hash, tr::string_eq>,
			  "const char* should fulfill tr::hash_keylike<std::string, tr::string_hash, tr::string_eq>, but does not!");

/// Tests `tr::hash_keylike` on a negative case.
static_assert(!tr::hash_keylike<int, std::string, tr::string_hash, tr::string_eq>,
			  "int should not fulfill tr::hash_keylike<std::string, tr::string_hash, tr::string_eq>, but does!");