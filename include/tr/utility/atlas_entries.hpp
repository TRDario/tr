/// @file
/// @brief Provides `tr::atlas_entries`.

#pragma once
#include <tr/utility/atlas_packer.hpp>
#include <tr/utility/hash_map.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/type_name.hpp>

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
		[[nodiscard]] atlas_entries() = default;

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the atlas contains a key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @return `true` if a value associated with `key` existed, `false` otherwise.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] bool contains(Keylike&& key) const
		{
			return m_entries.contains(std::forward<Keylike>(key));
		}

		/// Gets the number of entries in the atlas.
		/// @return Number of entries in the atlas.
		[[nodiscard]] usize entries() const noexcept
		{
			return m_entries.size();
		}

		/// @}
		/// @name Access
		/// @{

		/// Gets a rectangle associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
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
		/// @name Manipulation
		/// @{

		/// Clears the packer.
		void clear()
		{
			m_packer.clear();
			m_entries.clear();
		}

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
		std::optional<glm::u16vec2> try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size, Args&&... extra_args)
		{
			TR_ASSERT(!contains(std::forward<Keylike>(key)),
					  "Tried to insert a rectangle with the same key (type: '{}') as an existing rectangle into an atlas packer.",
					  type_name<Key>());

			std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
			if (packing_result.has_value()) {
				m_entries.emplace(std::piecewise_construct, std::forward_as_tuple(std::forward<Keylike>(key)),
								  std::forward_as_tuple(rectangle<u16>{*packing_result, size}, std::forward<Args>(extra_args)...));
			}
			return packing_result;
		}

		/// @}

	  private:
		/// Atlas packer.
		atlas_packer m_packer;

		/// Atlas entries.
		boost::unordered_flat_map<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr