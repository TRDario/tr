/// @file
/// @brief Provides `tr::localization_map`.

#pragma once
#include <tr/utility/hash_map.hpp>
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// Optimized localization key-value map.
	class localization_map
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an empty localization map.
		[[nodiscard]] localization_map() noexcept = default;

		/// Copies a localization map.
		/// @param rhs Localization map to copy.
		[[nodiscard]] localization_map(const localization_map& rhs);

		/// Moves a localization map.
		/// @details `rhs` will be left empty after the move.
		/// @param rhs Localization map to move.
		[[nodiscard]] localization_map(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Copies a localization map.
		/// @param rhs Localization map to copy.
		/// @return Reference to `*this`.
		localization_map& operator=(const localization_map& rhs);

		/// Moves a localization map.
		/// @details `rhs` will be left empty after the move.
		/// @param rhs Localization map to move.
		/// @return Reference to `*this`.
		localization_map& operator=(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Access
		/// @{

		/// Gets the number of entries in the map.
		/// @return Number of entries in the map.
		[[nodiscard]] usize size() const noexcept;

		/// Gets whether a key has a corresponding localization string in the map.
		/// @param key Localization key to check.
		/// @return `true` if a string is associated with `key`, `false` otherwise.
		[[nodiscard]] bool contains(std::string_view key) const noexcept;

		/// Gets a localization string associated with a key.
		/// @param key Localization key to get a localization string for.
		/// @post The string `key` is a view of must stay alive after the function returns in case it's returned.
		/// @return Localization string associated with a key, or `key` if one doesn't exist.
		[[nodiscard]] std::string_view operator[](std::string_view key) const noexcept;

		/// @}
		/// @name Modification
		/// @{

		/// Clears the localization map.
		void clear() noexcept;

		/// Reserves memory for up to `capacity` entries in the map.
		/// @param capacity Expected number of entries in the map.
		void reserve(usize capacity);

		/// Inserts a key-value pair into the localization map.
		/// @param key Localization key.
		/// @param value Localization value associated with `key`.
		void insert(std::string_view key, std::string_view value);

		/// @}

	  private:
		/// String storage pool.
		/// @details Key-value strings are stored interleaved separated by NUL.
		std::vector<char> m_string_pool;

		/// Set of keys present in the map.
		boost::unordered_flat_set<zstring_view, string_hash, string_eq> m_keys;

		//

		/// Finds the next key string after `it`.
		/// @param it Iterator to a key string.
		/// @return Iterator to the beginning of the next key string.
		std::vector<char>::const_iterator find_next_key(std::vector<char>::const_iterator it) const noexcept;

		/// Builds `m_keys` after an `m_string_pool` reallocation.
		void build_keys();
	};
} // namespace tr