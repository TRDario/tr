///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements hash_map.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../hash_map.hpp"

///////////////////////////////////////////////////////////////// GETTERS /////////////////////////////////////////////////////////////////

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
Value& tr::get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::iterator it{map.find(keylike)};
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

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
Value& tr::get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_node_map<Key, Value, Hash, Pred>::iterator it{map.find(keylike)};
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

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
const Value& tr::get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::const_iterator it{map.find(keylike)};
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

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
const Value& tr::get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_node_map<Key, Value, Hash, Pred>::const_iterator it{map.find(keylike)};
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

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
tr::opt_ref<Value> tr::try_get(boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::iterator it{map.find(keylike)};
	return it != map.end() ? tr::opt_ref<Value>{it->second} : std::nullopt;
}

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
tr::opt_ref<Value> tr::try_get(boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_node_map<Key, Value, Hash, Pred>::iterator it{map.find(keylike)};
	return it != map.end() ? tr::opt_ref<Value>{it->second} : std::nullopt;
}

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
tr::opt_ref<const Value> tr::try_get(const boost::unordered_flat_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_flat_map<Key, Value, Hash, Pred>::const_iterator it{map.find(keylike)};
	return it != map.end() ? tr::opt_ref<const Value>{it->second} : std::nullopt;
}

template <typename Key, typename Value, typename Hash, typename Pred, typename Keylike>
tr::opt_ref<const Value> tr::try_get(const boost::unordered_node_map<Key, Value, Hash, Pred>& map, Keylike&& keylike)
{
	const typename boost::unordered_node_map<Key, Value, Hash, Pred>::const_iterator it{map.find(keylike)};
	return it != map.end() ? tr::opt_ref<const Value>{it->second} : std::nullopt;
}