/// @file
/// @brief Provides miscellaneous hash map functionality.

#pragma once
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/static_string.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Transparent string hasher.
	struct string_hash : boost::hash<std::string_view>
	{
		/// Marks the hasher as transparent.
		using is_transparent = std::true_type;
	};

	/// Transparent string equality comparator.
	struct string_eq : std::equal_to<std::string_view>
	{
		/// Marks the hasher as transparent.
		using is_transparent = std::true_type;
	};

	//

	/// String-key flat map.
	/// @tparam Value Map value type.
	template <typename Value>
	using string_flat_map = boost::unordered_flat_map<std::string, Value, string_hash, string_eq>;

	/// String-key node map.
	/// @tparam Value Map value type.
	template <typename Value>
	using string_node_map = boost::unordered_node_map<std::string, Value, string_hash, string_eq>;

	/// Static string-key flat map.
	/// @tparam KeyCapacity String capacity of keys.
	/// @tparam Value Map value type.
	template <usize KeyCapacity, typename Value>
	using static_string_flat_map = boost::unordered_flat_map<static_string<KeyCapacity>, Value, string_hash, string_eq>;

	/// Static string-key node map.
	/// @tparam KeyCapacity String capacity of keys.
	/// @tparam Value Map value type.
	template <usize KeyCapacity, typename Value>
	using static_string_node_map = boost::unordered_node_map<static_string<KeyCapacity>, Value, string_hash, string_eq>;

	/// @name Hash maps
	/// @{

	/// Gets a value from a flat map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @pre `keylike` must exist in the map.
	/// @return Reference to the value under `keylike`.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] Value& get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::iterator it{map.find(std::forward<Keylike>(keylike))};
		if constexpr (valid_format_string_for<"{}", Keylike>) {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at nonexistant key '{}'.", type_name<Key>(),
					  type_name<Value>(), keylike);
		}
		else {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at a nonexistant key.", type_name<Key>(),
					  type_name<Value>());
		}
		return it->second;
	}

	/// Gets a value from a node map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @pre `keylike` must exist in the map.
	/// @return Reference to the value under `keylike`.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] Value& get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_node_map<Key, Value, Hash, Pred>::iterator it{map.find(std::forward<Keylike>(keylike))};
		if constexpr (valid_format_string_for<"{}", Keylike>) {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at nonexistant key '{}'.", type_name<Key>(),
					  type_name<Value>(), keylike);
		}
		else {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at a nonexistant key.", type_name<Key>(),
					  type_name<Value>());
		}
		return it->second;
	}

	/// Gets a value from a flat map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @pre `keylike` must exist in the map.
	/// @return Reference to the value under `keylike`.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] const Value& get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::const_iterator it{map.find(std::forward<Keylike>(keylike))};
		if constexpr (valid_format_string_for<"{}", Keylike>) {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at nonexistant key '{}'.", type_name<Key>(),
					  type_name<Value>(), keylike);
		}
		else {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at a nonexistant key.", type_name<Key>(),
					  type_name<Value>());
		}
		return it->second;
	}

	/// Gets a value from a node map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @pre `keylike` must exist in the map.
	/// @return Reference to the value under `keylike`.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] const Value& get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_node_map<Key, Value, Hash, Pred>::const_iterator it{map.find(std::forward<Keylike>(keylike))};
		if constexpr (valid_format_string_for<"{}", Keylike>) {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at nonexistant key '{}'.", type_name<Key>(),
					  type_name<Value>(), std::forward<Keylike>(keylike));
		}
		else {
			TR_ASSERT(it != map.end(), "Tried to get a value from a '{}' -> '{}' hash map at a nonexistant key.", type_name<Key>(),
					  type_name<Value>());
		}
		return it->second;
	}

	//

	/// Tries to get a value from a flat map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @return Reference to the value under `keylike`, or an empty optional reference if not found.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] opt_ref<Value> try_get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::iterator it{map.find(std::forward<Keylike>(keylike))};
		return it != map.end() ? opt_ref<Value>{it->second} : std::nullopt;
	}

	/// Tries to get a value from a node map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @return Reference to the value under `keylike`, or an empty optional reference if not found.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] opt_ref<Value> try_get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_node_map<Key, Value, Hash, Pred>::iterator it{map.find(std::forward<Keylike>(keylike))};
		return it != map.end() ? opt_ref<Value>{it->second} : std::nullopt;
	}

	/// Tries to get a value from a flat map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @return Reference to the value under `keylike`, or an empty optional reference if not found.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] opt_ref<const Value> try_get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::const_iterator it{map.find(std::forward<Keylike>(keylike))};
		return it != map.end() ? opt_ref<const Value>{it->second} : std::nullopt;
	}

	/// Tries to get a value from a node map.
	/// @tparam Key Map key type.
	/// @tparam Value Map value type.
	/// @tparam Hash Map hasher type.
	/// @tparam Pred Map equality predicate type.
	/// @tparam Keylike Type compatible with the hash map for lookup.
	/// @param map Map to get the value from.
	/// @param keylike Keylike value to look for.
	/// @return Reference to the value under `keylike`, or an empty optional reference if not found.
	template <typename Key, typename Value, hasher<Key> Hash, equality_predicate<Key> Pred, hash_keylike<Key, Hash, Pred> Keylike>
	[[nodiscard]] opt_ref<const Value> try_get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
	{
		const typename boost::unordered_node_map<Key, Value, Hash, Pred>::const_iterator it{map.find(std::forward<Keylike>(keylike))};
		return it != map.end() ? opt_ref<const Value>{it->second} : std::nullopt;
	}

	/// @}
} // namespace tr