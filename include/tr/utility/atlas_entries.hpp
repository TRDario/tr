/// @file
/// @brief Provides `tr::atlas_entries` and related functionality.

/// @ingroup containers
/// @defgroup atlas_entries Atlas entries
/// @brief Contains `tr::atlas_entries` and related functionality.

#pragma once
#include <tr/utility/atlas_packer.hpp>
#include <tr/utility/hash_map.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @ingroup atlas_entries
	/// @brief Concept checking for a valid `tr::atlas_entries` value type.
	/// @details Must be a `rectangle<u16>` or a structure containing a member `.uv` of type `rectangle<u16>`.
	/// @tparam Target Type to check for being a valid `tr::atlas_entries` value type.
	template <typename Target>
	concept atlas_entries_value_type = std::same_as<Target, rectangle<u16>> || requires(const Target& v) {
		{ v.uv } -> std::same_as<const rectangle<u16>&>;
	};

	//

	/// @ingroup atlas_entries
	/// @brief Atlas packer combined with a list of entries.
	/// @details `tr::atlas_entries` is designed to be used in tandem with a sized texture to build a proper atlas. As such, it does not
	/// hold information about texture size within itself, and expects it to be passed in to `try_insert()`. In order to ensure the results
	/// of the algorithm are valid, the texture size passed into `try_insert()` must always be equal to or greater than its value during a
	/// previous call. That is to say, the algorithm expects that the atlas texture can only ever grow, not shrink.
	/// @tparam Key Entry key type.
	/// @tparam Value Entry value type.
	/// @tparam Hash Entry key hasher.
	/// @tparam Pred Entry key equality predicate.
	template <typename Key, atlas_entries_value_type Value = rectangle<u16>, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	class atlas_entries
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty atlas.
		[[nodiscard]] atlas_entries() = default;

		/// @}
		/// @name Capacity
		/// @{

		/// @brief Returns the number of entries in the atlas.
		/// @return Number of entries in the atlas.
		[[nodiscard]] usize size() const noexcept
		{
			return m_entries.size();
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Clears the packer.
		void clear() noexcept
		{
			m_packer.clear();
			m_entries.clear();
		}

		/// @brief Attempts to insert a rectangle and returns the top-left corner of the rectangle if successful.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @tparam Args Extra argument types.
		/// @param key Key to associate with the rectangle.
		/// @param size Size of the rectangle.
		/// @param texture_size Size of the texture to insert the rectangle into.
		/// @param extra_args Extra arguments associated with the rectangle value.
		/// @pre `texture_size` must be equal to or greater than its value on the previous `try_insert()` call. When calling for the first
		/// time after construction or clearing, it may be any value, including (0, 0).
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return Position of the top-left corner of the rectangle, if found.
		template <hash_keylike<Key, Hash, Pred> Keylike, typename... Args>
			requires std::constructible_from<Value, tr::rectangle<u16>, Args...>
		std::optional<glm::u16vec2> try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size, Args&&... extra_args)
		{
			TR_ASSERT(!contains(std::forward<Keylike>(key)),
					  "Tried to insert a rectangle with the same key (type: '{}') as an existing rectangle into an atlas packer.",
					  type_name<Key>());

			try {
				std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
				if (packing_result) {
					m_entries.emplace(std::piecewise_construct, std::forward_as_tuple(std::forward<Keylike>(key)),
									  std::forward_as_tuple(rectangle<u16>{*packing_result, size}, std::forward<Args>(extra_args)...));
				}
				return packing_result;
			}
			catch (std::exception& error) {
				clear();
				throw error;
			}
		}

		/// @}
		/// @name Lookup
		/// @{

		/// @brief Checks whether the atlas contains a key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return `true` if a value associated with `key` existed, `false` otherwise.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] bool contains(Keylike&& key) const
		{
			return m_entries.contains(std::forward<Keylike>(key));
		}

		/// @brief Returns a rectangle associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Constant reference to the value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] const Value& operator[](Keylike&& key) const
		{
			if constexpr (valid_format_string_for<"{}", Keylike>) {
				TR_ASSERT(contains(std::forward<Keylike>(key)), "Tried to get a rectangle at nonexistant key '{}' from an atlas packer.",
						  key);
			}
			else {
				TR_ASSERT(contains(std::forward<Keylike>(key)),
						  "Tried to get a rectangle at a nonexistant key (type: '{}') from an atlas packer.", type_name<Key>());
			}

			return get(m_entries, std::forward<Keylike>(key));
		}

		/// @}

	  private:
		/// @brief Atlas packer algorithm object.
		atlas_packer m_packer;

		/// @brief Map of atlas entries.
		boost::unordered_flat_map<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr