/// @file
/// @brief Provides hashing concepts.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Hasher compatible with a specific type.
	/// @tparam Hashed Type that must be hashable.
	template <typename T, typename Hashed>
	concept hasher = requires(const Hashed& v) {
		{ T{}(v) } -> std::convertible_to<u64>;
	};

	/// Equality predicate compatible with a specific type.
	/// @tparam Lhs Type on the left-hand side of the equality operation.
	/// @tparam Rhs Type on the right-hand side of the equality operation.
	template <typename T, typename Lhs, typename Rhs = Lhs>
	concept equality_predicate = requires(const Lhs& lhs, const Rhs& rhs) {
		{ T{}(lhs, rhs) } -> std::same_as<bool>;
	};

	/// "Keylike" type compatible with a hash collection.
	/// @tparam Key Primary key type of the collection.
	/// @tparam Hash Hasher type of the collection.
	/// @tparam Eq Equality predicate type of the collection.
	template <typename T, typename Key, typename Hash, typename Eq>
	concept hash_keylike = hasher<Hash, T> && equality_predicate<Eq, Key, T>;
} // namespace tr