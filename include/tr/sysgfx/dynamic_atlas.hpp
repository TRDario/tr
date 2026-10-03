/// @file
/// @brief Provides `tr::dynamic_atlas`.

#pragma once
#include <tr/sysgfx/bitmap_atlas.hpp>
#include <tr/sysgfx/mutable_texture_view.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>
#include <tr/sysgfx/texture.hpp>
#include <tr/sysgfx/texture_view.hpp>

//

namespace tr
{
	/// Dynamically-allocated texture atlas.
	/// @details
	/// `tr::dynamic_atlas` is an abstraction over a `tr::texture` that allows for dynamic allocation of sub-textures within the texture.
	/// It is intended to be used as a texture atlas, where each entry in the atlas is associated with a key and a rectangle within the
	/// texture.
	///
	/// Strictly speaking, instances of `tr::dynamic_atlas` are only containers for these underlying textures. This means, for example,
	/// that setting a dynamic atlas as a texture on a shader uniform does not associate it with a literal `tr::dynamic_atlas` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of `tr::dynamic_atlas`,
	/// that value will still be set on the shader uniform. If the instance is overriden with a new value, the old value is destroyed and
	/// the shader will no longer have a set texture on that uniform. In addition, modifying the atlas may reallocate the underlying
	/// texture, which invalidates any texture views that were previously obtained from it.
	///
	/// Every instance of `tr::dynamic_atlas` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_atlas` are left in a special 'invalid' state. Invalid `tr::dynamic_atlas` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::dynamic_atlas` instances may be labeled and are formattable. Example format output: `"My atlas" (Texture OpenGL ID: 5)`.
	/// @tparam Key Atlas key type.
	/// @tparam Value Atlas value type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	template <typename Key, atlas_entries_value_type Value, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	class dynamic_atlas
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty atlas.
		/// @param context Graphics context to create the atlas on.
		[[nodiscard]] explicit dynamic_atlas(graphics_context& context) noexcept
			: m_texture{context}
		{
		}

		/// Creates an empty atlas with an initial size.
		/// @param context Graphics context to create the atlas on.
		/// @param size Initial size of the atlas.
		[[nodiscard]] dynamic_atlas(graphics_context& context, glm::ivec2 size)
			: m_texture{context, size, mipmaps::enabled}
		{
		}

		/// Uploads a bitmap atlas.
		/// @param context Graphics context to create the atlas on.
		/// @param source Bitmap atlas to use as a source.
		[[nodiscard]] dynamic_atlas(graphics_context& context, bitmap_atlas<Key, Value, Hash, Pred>&& source)
			: m_texture{context, source.bitmap, mipmaps::enabled}
			, m_entries{std::move(source.rectangles)}
		{
		}

		/// Dynamic atlases are not copyable.
		dynamic_atlas(const dynamic_atlas&) = delete;

		/// Moves a dynamic atlas.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic atlas to move.
		[[nodiscard]] dynamic_atlas(dynamic_atlas&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Dynamic atlases are not copyable.
		dynamic_atlas& operator=(const dynamic_atlas&) = delete;

		/// Moves a dynamic atlas.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic atlas to move.
		/// @return Reference to `*this`.
		dynamic_atlas& operator=(dynamic_atlas&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the atlas is on.
		/// @return Reference to the graphics context the atlas is on.
		[[nodiscard]] graphics_context& context() const noexcept
		{
			return m_texture.context();
		}

		/// @}
		/// @name State
		/// @{

		/// Gets whether the atlas is in a valid state.
		/// @return `true` if the atlas is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept
		{
			return m_texture.valid();
		}

		/// Gets whether the atlas texture is complete.
		/// @return `true` if the atlas texture is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept
		{
			return m_texture.complete();
		}

		/// Gets the size of the atlas texture.
		/// @return Size of the atlas texture.
		[[nodiscard]] glm::ivec2 size() const noexcept
		{
			return m_texture.size();
		}

		/// @}
		/// @name Texture
		/// @{

		/// Gets the atlas texture.
		/// @return Reference to the atlas texture.
		[[nodiscard]] operator const texture&() const noexcept
		{
			return m_texture;
		}

		/// Gets a mutable view to the atlas texture.
		/// @note This texture view stays valid until a call to `reserve()`, `add()`, `clear()`, or the destruction of the atlas.
		/// @return Mutable view to the atlas texture.
		[[nodiscard]] operator mutable_texture_view() noexcept
		{
			return m_texture;
		}

		/// Gets a view to the atlas texture.
		/// @note This texture view stays valid until a call to `reserve()`, `add()`, `clear()`, or the destruction of the atlas.
		/// @return View to the atlas texture.
		[[nodiscard]] operator texture_view() const noexcept
		{
			return m_texture;
		}

		/// Gets the atlas texture.
		/// @return Reference to the atlas texture.
		[[nodiscard]] const texture& texture() const noexcept
		{
			return m_texture;
		}

		/// Gets a mutable view to the atlas texture.
		/// @note This texture view stays valid until a call to `reserve()`, `add()`, `clear()`, or the destruction of the atlas.
		/// @return Mutable view to the atlas texture.
		[[nodiscard]] mutable_texture_view view() noexcept
		{
			return m_texture;
		}

		/// Gets a view to the atlas texture.
		/// @note This texture view stays valid until a call to `reserve()`, `add()`, `clear()`, or the destruction of the atlas.
		/// @return View to the atlas texture.
		[[nodiscard]] texture_view view() const noexcept
		{
			return m_texture;
		}

		/// @}
		/// @name Attributes
		/// @{

		/// Sets the filters used by the atlas texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
		{
			m_texture.set_filtering(min_filter, mag_filter);
		}

		/// @}
		/// @name Information
		/// @{

		/// Gets whether the atlas contains an entry.
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
			return m_entries.entries();
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
		[[nodiscard]] rectangle<float> operator[](Keylike&& key) const
		{
			rectangle<float> uv;
			if constexpr (std::same_as<Value, rectangle<u16>>) {
				uv = m_entries[std::forward<Keylike>(key)];
			}
			else {
				uv = m_entries[std::forward<Keylike>(key)].uv;
			}

			uv.tl /= size();
			uv.size /= size();
			return uv;
		}

		/// Gets the raw value associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
		/// @return Constant reference to the raw value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] const Value& raw(Keylike&& key) const
		{
			TR_ASSERT(contains(key), "Tried to get nonexistent dynamic atlas entry.");

			return m_entries[std::forward<Keylike>(key)];
		}

		/// @}
		/// @name Manipulation
		/// @{

		/// Reserves a certain amount of space in the bitmap.
		/// @param capacity New size of the atlas bitmap.
		void reserve(glm::ivec2 capacity)
		{
			if (capacity.x < size().x && capacity.y < size().y) {
				return;
			}
			else if (m_texture.size() == glm::ivec2{}) {
				m_texture.allocate(capacity);
				m_texture.clear({});
			}
			else {
				const tr::texture old_tex{m_texture.allocate(capacity)};
				m_texture.clear({});
				m_texture.copy_region({}, old_tex, {{}, old_tex.size()});
			}
		}

		/// Adds an entry to the atlas.
		/// @tparam Args Extra argument types.
		/// @param key Key to associate with the entry.
		/// @param bitmap Bitmap to copy into the atlas.
		/// @param args Extra arguments associated with the entry.
		template <typename... Args>
			requires(std::constructible_from<Value, rectangle<u16>, Args...>)
		void add(Key key, sub_bitmap bitmap, Args&&... args)
		{
			std::optional<glm::u16vec2> tl{m_entries.try_insert(std::move(key), bitmap.size(), size(), std::forward<Args>(args)...)};
			if (!tl.has_value()) {
				glm::u16vec2 new_size;
				if (m_texture.size() == glm::ivec2{}) {
					const glm::uvec2 usize{bitmap.size()};
					new_size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
					tl = m_entries.try_insert(std::move(key), usize, new_size, std::forward<Args>(args)...);
				}
				else {
					new_size = size();
					do {
						new_size.y < new_size.x ? new_size.y *= 2 : new_size.x *= 2;
					} while (
						!(tl = m_entries.try_insert(std::move(key), bitmap.size(), new_size, std::forward<Args>(args)...)).has_value());
				}
				reserve(new_size);
			}
			m_texture.set_region(*tl, bitmap);
		}

		/// Removes all entries from the atlas.
		void clear()
		{
			if (size() != glm::ivec2{0}) {
				m_texture.clear();
				m_entries.clear();
			}
		}

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the atlas.
		/// @return Label of the atlas.
		[[nodiscard]] std::string label() const
		{
			return m_texture.label();
		}

		/// Sets the debug label of the atlas.
		/// @param label Label of the atlas.
		void set_label(std::string_view label) noexcept
		{
			m_texture.set_label(label);
		}

		/// @}

	  private:
		/// Atlas texture.
		tr::texture m_texture;

		/// Atlas entries.
		atlas_entries<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr

//

/// Dynamic atlas formatter.
template <typename Key, tr::atlas_entries_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
struct std::formatter<tr::dynamic_atlas<Key, Value, Hash, Pred>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid dynamic atlas format specification."};
		}
		return context.begin();
	}

	/// Formats a dynamic atlas.
	/// @tparam FormatContext Formatting context type.
	/// @param atlas Atlas to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::dynamic_atlas<Key, Value, Hash, Pred>& atlas, FormatContext& context) const
	{
		if (atlas.valid()) {
			return std::format_to(context.out(), "\"{}\" (Texture OpenGL ID: {})", atlas.label(), atlas.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid dynamic atlas at {}>", static_cast<const void*>(&atlas));
		}
	}
};