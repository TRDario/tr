/// @file
/// @brief Provides utilities for packing textures into an atlas.

#pragma once
#include "rectangle.hpp"

//

namespace tr
{
	/// Valid `tr::atlas_entries` value type.
	/// @details Must be a `rectangle<u16>` or a structure containing a member `.uv` of type `rectangle<u16>`.
	template <typename T>
	concept atlas_entries_value_type = std::same_as<T, rectangle<u16>> || requires(const T& v) {
		{ v.uv } -> std::same_as<const rectangle<u16>&>;
	};

	//

	/// Skyline rectangle packer for atlas textures.
	/// @note The packer does not keep track of texture information and assumes it is of constant size or growing.
	/// @note All entries are given a pixel of leeway in both dimensions to prevent filtering bleed.
	class atlas_packer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty packer.
		atlas_packer();

		/// @}
		/// @name Manipulation
		/// @{

		/// Clears the packer.
		void clear();

		/// Attempts to insert a rectangle.
		/// @param size Size of the rectangle.
		/// @param texture_size Size of the texture to insert the rectangle into.
		/// @return Position of the top-left corner of the rectangle, if found.
		std::optional<glm::u16vec2> try_insert(glm::u16vec2 size, glm::u16vec2 texture_size);

		/// @}

	  private:
		/// The skyline silhouette points.
		std::vector<glm::u16vec2> m_skyline;
	};

	//

	/// Atlas packer combined with a list of entries.
	/// @tparam Key Entry key type.
	/// @tparam Value Entry value type.
	/// @tparam Hash Entry key hasher.
	/// @tparam Pred Entry key equality predicate.
	template <typename Key, atlas_entries_value_type Value = rectangle<u16>, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	class atlas_entries
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty atlas.
		atlas_entries() = default;

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the atlas contains a key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @return `true` if a value associated with `key` existed, `false` otherwise.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		bool contains(Keylike&& key) const;

		/// Gets the number of entries in the atlas.
		/// @return Number of entries in the atlas.
		usize entries() const;

		/// @}
		/// @name Access
		/// @{

		/// Gets a rectangle associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
		/// @return Constant reference to the value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		const Value& operator[](Keylike&& key) const;

		/// @}
		/// @name Manipulation
		/// @{

		/// Clears the packer.
		void clear();

		/// Tries to insert a rectangle and returns the top-left corner of the rectangle if successful.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @tparam Args Extra argument types.
		/// @param key Key to associate with the rectangle.
		/// @param size Size of the rectangle.
		/// @param texture_size Size of the texture to insert the rectangle into.
		/// @param extra_args Extra arguments associated with the rectangle value.
		/// @return Position of the top-left corner of the rectangle, if found.
		template <hash_keylike<Key, Hash, Pred> Keylike, typename... Args>
			requires(std::constructible_from<Value, tr::rectangle<u16>, Args...>)
		std::optional<glm::u16vec2> try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size, Args&&... extra_args);

		/// @}

	  private:
		/// Atlas packer.
		atlas_packer m_packer;

		/// Atlas entries.
		boost::unordered_flat_map<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr

#include "impl/atlas_packer.hpp" // IWYU pragma: export