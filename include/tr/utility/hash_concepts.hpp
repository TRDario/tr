/// @file
/// @brief Provides hashing concepts.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Hasher compatible with a specific type.
	/// @tparam Hashed Type that must be hashable.
	template <typename Target, typename Hashed>
	concept hasher = std::default_initializable<Target> && requires(Target hasher, const Hashed& value) {
		{ hasher(value) } -> std::convertible_to<u64>;
	};

	/// Equality predicate compatible with a specific type.
	/// @tparam Lhs Type on the left-hand side of the equality operation.
	/// @tparam Rhs Type on the right-hand side of the equality operation.
	template <typename Target, typename Lhs, typename Rhs = Lhs>
	concept equality_predicate = std::default_initializable<Target> && requires(Target pred, const Lhs& lhs, const Rhs& rhs) {
		{ pred(lhs, rhs) } -> std::same_as<bool>;
	};

	/// "Keylike" type compatible with a hash collection.
	/// @tparam Key Primary key type of the collection.
	/// @tparam Hash Hasher type of the collection.
	/// @tparam Eq Equality predicate type of the collection.
	template <typename Target, typename Key, typename Hash, typename Eq>
	concept hash_keylike = hasher<Hash, Target> && equality_predicate<Eq, Key, Target>;
} // namespace tr