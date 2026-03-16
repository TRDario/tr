///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous hash map functionality.                                                                                        //
//                                                                                                                                       //
// tr::string_hash and tr::string_eq are provided as transparent alternatives for string hashing and comparison.                         //
// The aliases tr::string_hash_map and tr::static_string_hash_map use these types to allow searching with std::string_view or const char*//
// instead of having to allocate an std::string:                                                                                         //
//     - tr::string_hash_map<int> test; test.find("key")->second = 10 -> Doesn't allocate a std::string like it usually would.           //
//                                                                                                                                       //
// Values can be gotten from hash maps using heterogenous key lookup using tr::get. tr::try_get may be used to get optional references   //
// in cases where a missing key is an expected scenario:                                                                                 //
//     - tr::string_hash_map<int> test{{"a", 1}}; tr::get(test, "a") -> 1                                                                //
//     - tr::string_hash_map<int> test{{"a", 1}}; tr::try_get(test, "b") -> std::nullopt                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "reference.hpp"
#include "static_string.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Transparent string hasher.
	struct string_hash : boost::hash<std::string_view> {
		using is_transparent = std::true_type;
	};
	// Transparent string equality comparator.
	struct string_eq : std::equal_to<std::string_view> {
		using is_transparent = std::true_type;
	};

	// Typedef for a string-key flat map.
	template <typename Value> using string_flat_map = boost::unordered_flat_map<std::string, Value, string_hash, string_eq>;
	// Typedef for a string-key node map.
	template <typename Value> using string_node_map = boost::unordered_node_map<std::string, Value, string_hash, string_eq>;
	// Typedef for a static string-key flat map.
	template <usize KeyCapacity, typename Value>
	using static_string_flat_map = boost::unordered_flat_map<static_string<KeyCapacity>, Value, string_hash, string_eq>;
	// Typedef for a static string-key node map.
	template <usize KeyCapacity, typename Value>
	using static_string_node_map = boost::unordered_node_map<static_string<KeyCapacity>, Value, string_hash, string_eq>;

	// Gets a value from a flat map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	Value& get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Gets a value from a node map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	Value& get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Gets a value from a flat map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	const Value& get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Gets a value from a node map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	const Value& get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Tries to get a value from a flat map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	opt_ref<Value> try_get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Tries to get a value from a node map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	opt_ref<Value> try_get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Tries to get a value from a flat map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	opt_ref<const Value> try_get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
	// Tries to get a value from a mode map.
	template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
	opt_ref<const Value> try_get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike);
} // namespace tr

#include "impl/hash_map.hpp" // IWYU pragma: export