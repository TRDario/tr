/// @file
/// @brief Provides hashing concepts.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Concept checking for a hasher compatible with a specific type.
	/// @tparam Target Type to check for being a hasher to `Hashed`.
	/// @tparam Hashed Type that must be hashable.
	template <typename Target, typename Hashed>
	concept hasher = std::default_initializable<Target> && requires(Target hasher, const Hashed& value) {
		{ hasher(value) } -> std::convertible_to<u64>;
	};

	/// @ingroup meta
	/// @brief Concept checking for an equality predicate compatible with a specific type.
	/// @tparam Target Type to check for being an equality predicate.
	/// @tparam Lhs Type on the left-hand side of the equality operation.
	/// @tparam Rhs Type on the right-hand side of the equality operation.
	template <typename Target, typename Lhs, typename Rhs = Lhs>
	concept equality_predicate = std::default_initializable<Target> && requires(Target pred, const Lhs& lhs, const Rhs& rhs) {
		{ pred(lhs, rhs) } -> std::same_as<bool>;
	};

	/// @ingroup meta
	/// @brief Concept checking for a "keylike" type compatible with a hash collection.
	/// @tparam Target Type to check for being "keylike".
	/// @tparam Key Primary key type of the collection.
	/// @tparam Hash Hasher type of the collection.
	/// @tparam Eq Equality predicate type of the collection.
	template <typename Target, typename Key, typename Hash, typename Eq>
	concept hash_keylike = hasher<Hash, Target> && equality_predicate<Eq, Key, Target>;
} // namespace tr