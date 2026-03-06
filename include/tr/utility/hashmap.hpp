///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous hashmap functionality.                                                                                         //
//                                                                                                                                       //
// tr::string_hash and tr::string_eq are provided as transparent alternatives for string hashing and comparison.                         //
// The aliases tr::string_hash_map and tr::static_string_hash_map use these types to allow searching with std::string_view or const char*//
// instead of having to allocate an std::string:                                                                                         //
//     - tr::string_hash_map<int> test; *test.find("key") = 10 -> Doesn't allocate a std::string like it usually would.                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "static_string.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Transparent string hasher.
	struct string_hash : std::hash<std::string_view> {
		using is_transparent = std::true_type;
	};
	// Transparent string equality comparator.
	struct string_eq : std::equal_to<std::string_view> {
		using is_transparent = std::true_type;
	};

	// Typedef for a string-key hash map.
	template <typename Value> using string_hash_map = std::unordered_map<std::string, Value, string_hash, string_eq>;
	// Typedef for a static string-key hash map.
	template <usize KeyCapacity, typename Value>
	using static_string_hash_map = std::unordered_map<static_string<KeyCapacity>, Value, string_hash, string_eq>;
} // namespace tr