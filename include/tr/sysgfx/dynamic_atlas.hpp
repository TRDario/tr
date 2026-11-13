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
	/// @brief Owning container for a dynamically-allocated texture atlas.
	/// @details
	/// `tr::dynamic_atlas` is an abstraction over a `tr::texture` that allows for dynamic allocation of sub-textures within the texture.
	/// It is intended to be used as a texture atlas, where each entry in the atlas is associated with a key and a rectangle within the
	/// texture.
	///
	/// It's worth remembering that instances of `tr::dynamic_atlas` are only containers for these underlying textures. This means, for
	/// example, that setting a dynamic atlas as a texture on a shader uniform does not associate it with a literal `tr::dynamic_atlas`
	/// object at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::dynamic_atlas`, that value will still be set on the shader uniform. If the instance is overriden with a new value, the old
	/// value is destroyed and the shader will no longer have a set texture on that uniform. In addition, modifying the atlas may reallocate
	/// the underlying texture, which invalidates any texture views that were previously obtained from it.
	///
	/// Every instance of `tr::dynamic_atlas` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_atlas` are left in a special 'invalid' state. Invalid `tr::dynamic_atlas` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
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
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty atlas.
		/// @param context Graphics context to create the atlas on.
		/// @param label Label of the atlas.
		[[nodiscard]] explicit dynamic_atlas(graphics_context& context, std::string_view label = {}) noexcept
			: m_texture{context, label}
		{
		}

		/// @brief Constructs an empty atlas with an initial size.
		/// @param context Graphics context to create the atlas on.
		/// @param size Initial size of the atlas.
		/// @param label Label of the atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] dynamic_atlas(graphics_context& context, glm::ivec2 size, std::string_view label = {})
			: m_texture{context, size, mipmaps::enabled, tr::pixel_format::rgba32, label}
		{
		}

		/// @brief Constructs an atlas from a bitmap atlas.
		/// @param context Graphics context to create the atlas on.
		/// @param source Bitmap atlas to use as a source.
		/// @param label Label of the atlas.
		/// @par Complexity
		/// Linear time with respect to the size of the bitmap atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] dynamic_atlas(graphics_context& context, bitmap_atlas<Key, Value, Hash, Pred>&& source, std::string_view label = {})
			: m_texture{context, source.bitmap, mipmaps::enabled, tr::pixel_format::rgba32, label}
			, m_entries{std::move(source.rectangles)}
		{
		}

		/// @brief Dynamic atlases are not copyable.
		dynamic_atlas(const dynamic_atlas&) = delete;

		/// @brief Dynamic atlases are not copyable.
		dynamic_atlas& operator=(const dynamic_atlas&) = delete;

		/// @brief Constructs a dynamic atlas by transferring ownership from another `tr::dynamic_atlas`.
		/// @param rhs Dynamic atlas to move.
		/// @par Invalidation
		/// Any texture views and references to objects previously stored in `rhs` stay valid, but now point to the equivalent objects in
		/// `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] dynamic_atlas(dynamic_atlas&& rhs) noexcept = default;

		/// @brief Destroys the currently held dynamic atlas and replaces it with one moved from another `tr::dynamic_atlas`.
		/// @param rhs Dynamic atlas to move.
		/// @par Invalidation
		/// Any texture views and references to objects previously stored in `*this` are invalidated. If the objects were set anywhere, they
		/// are considered unset.
		///
		/// Any texture views and references to objects previously stored in `rhs` stay valid, but now point to the equivalent objects in
		/// `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		dynamic_atlas& operator=(dynamic_atlas&& rhs) noexcept = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a mutable view to the atlas texture.
		/// @pre The atlas must be in a valid state.
		/// @return Mutable view to the atlas texture.
		[[nodiscard]] mutable_texture_view view() noexcept
		{
			return m_texture;
		}

		/// @brief Returns a view to the atlas texture.
		/// @pre The atlas must be in a valid state.
		/// @return View to the atlas texture.
		[[nodiscard]] texture_view view() const noexcept
		{
			return m_texture;
		}

		/// @brief Returns a mutable view to the atlas texture.
		/// @pre The atlas must be in a valid state.
		/// @return Mutable view to the atlas texture.
		[[nodiscard]] operator mutable_texture_view() noexcept
		{
			return m_texture;
		}

		/// @brief Returns a view to the atlas texture.
		/// @pre The atlas must be in a valid state.
		/// @return View to the atlas texture.
		[[nodiscard]] operator texture_view() const noexcept
		{
			return m_texture;
		}

		/// @}
		/// @name Capacity
		/// @{

		/// @brief Returns the number of entries in the atlas.
		/// @pre The atlas must be in a valid state.
		/// @return Number of entries in the atlas.
		[[nodiscard]] usize size() const noexcept
		{
			TR_ASSERT(valid(), "Tried to get size of a dynamic atlas in an invalid state.");

			return m_entries.entries();
		}

		/// @brief Returns the size of the atlas texture.
		/// @pre The atlas must be in a valid state.
		/// @return Size of the atlas texture.
		[[nodiscard]] glm::ivec2 texture_size() const noexcept
		{
			TR_ASSERT(valid(), "Tried to get texture size of a dynamic atlas in an invalid state.");

			return m_texture.size();
		}

		/// @brief Reserves a certain amount of space in the atlas.
		/// @param texture_size New size of the atlas texture.
		/// @par Invalidation
		/// All texture views to the texture of `*this` are invalidated. If the texture was set anywhere, it is considered unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void reserve(glm::ivec2 texture_size)
		{
			TR_ASSERT(valid(), "Tried to reserve a texture for a dynamic atlas in an invalid state.");

			const glm::ivec2 old_texture_size{this->texture_size()};
			if (texture_size.x < old_texture_size.x && texture_size.y < old_texture_size.y) {
				return;
			}
			else if (old_texture_size == glm::ivec2{0, 0}) {
				m_texture.allocate(texture_size);
				m_texture.clear();
			}
			else {
				const tr::texture old_texture{m_texture.allocate(texture_size)};
				m_texture.clear();
				m_texture.copy_region(glm::vec2{0, 0}, old_texture, rectangle<int>{old_texture_size});
			}
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the filters used by the atlas texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		/// @pre The atlas must be in a valid state.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
		{
			TR_ASSERT(valid(), "Tried to set filtering of a dynamic atlas in an invalid state.");

			m_texture.set_filtering(min_filter, mag_filter);
		}

		/// @brief Removes all entries from the atlas.
		/// @pre The atlas must be in a valid state.
		/// @par Invalidation
		/// All references to entries stored in `*this` are invalidated.
		void clear() noexcept
		{
			TR_ASSERT(valid(), "Tried to clear a dynamic atlas in an invalid state.");

			if (complete()) {
				m_texture.clear();
				m_entries.clear();
			}
		}

		/// @brief Adds an entry to the atlas.
		/// @tparam Args Extra argument types.
		/// @param key Key to associate with the entry.
		/// @param bitmap Bitmap to copy into the atlas.
		/// @param args Extra arguments associated with the entry.
		/// @pre The atlas must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		template <typename... Args>
			requires std::constructible_from<Value, rectangle<u16>, Args...>
		void add(Key key, sub_bitmap bitmap, Args&&... args)
		{
			TR_ASSERT(valid(), "Tried to add entry to a dynamic atlas in an invalid state.");

			std::optional<glm::u16vec2> tl{
				m_entries.try_insert(std::move(key), bitmap.size(), texture_size(), std::forward<Args>(args)...),
			};
			if (!tl) {
				glm::u16vec2 new_size;
				if (m_texture.size() == glm::ivec2{0, 0}) {
					const glm::uvec2 usize{bitmap.size()};
					new_size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
					tl = m_entries.try_insert(std::move(key), usize, new_size, std::forward<Args>(args)...);
				}
				else {
					new_size = texture_size();
					do {
						new_size.y < new_size.x ? new_size.y *= 2 : new_size.x *= 2;
					} while (!(tl = m_entries.try_insert(std::move(key), bitmap.size(), new_size, std::forward<Args>(args)...)));
				}
				reserve(new_size);
			}
			m_texture.set_region(*tl, bitmap);
		}

		/// @brief Sets the label of the atlas.
		/// @param label Label of the atlas.
		/// @pre The atlas must be in a valid state.
		void set_label(std::string_view label) noexcept
		{
			TR_ASSERT(valid(), "Tried to set the label of a dynamic atlas in an invalid state.");

			m_texture.set_label(label);
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the atlas is in a valid state.
		/// @return `true` if the atlas is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept
		{
			return m_texture.valid();
		}

		/// @brief Checks whether the atlas is in a valid state.
		/// @return `true` if the atlas is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept
		{
			return m_texture.valid();
		}

		/// @brief Returns a reference to the graphics context the atlas is on.
		/// @pre The atlas must be in a valid state.
		/// @return Reference to the graphics context the atlas is on.
		[[nodiscard]] graphics_context& context() const noexcept
		{
			TR_ASSERT(valid(), "Tried to get context of a dynamic atlas in an invalid state.");

			return m_texture.context();
		}

		/// @brief Checks whether the atlas texture is complete.
		/// @pre The atlas must be in a valid state.
		/// @return `true` if the atlas texture is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept
		{
			TR_ASSERT(valid(), "Tried to check completeness of a dynamic atlas in an invalid state.");

			return m_texture.complete();
		}

		/// @brief Returns the label of the atlas.
		/// @pre The atlas must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the atlas.
		[[nodiscard]] std::string label() const
		{
			return m_texture.label();
		}

		/// @}
		/// @name Lookup
		/// @{

		/// @brief Checks whether the atlas contains an entry.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre The atlas must be in a valid state.
		/// @return `true` if a value associated with `key` existed, `false` otherwise.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] bool contains(Keylike&& key) const
		{
			TR_ASSERT(valid(), "Tried to check for the existence of an entry in a dynamic atlas in an invalid state.");

			return m_entries.contains(std::forward<Keylike>(key));
		}

		/// @brief Returns a rectangle associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre The atlas must be in a valid state and `key` must be present in the atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Constant reference to the value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] rectangle<float> operator[](Keylike&& key) const
		{
			TR_ASSERT(valid(), "Tried to get entry in a dynamic atlas in an invalid state.");
			TR_ASSERT(contains(key), "Tried to get nonexistent entry in dynamic atlas {}.", *this);

			rectangle<float> uv;
			if constexpr (std::same_as<Value, rectangle<u16>>) {
				uv = m_entries[std::forward<Keylike>(key)];
			}
			else {
				uv = m_entries[std::forward<Keylike>(key)].uv;
			}

			uv.tl /= texture_size();
			uv.size /= texture_size();
			return uv;
		}

		/// @brief Returns the raw value associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre The atlas must be in a valid state and `key` must be present in the atlas.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Constant reference to the raw value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		[[nodiscard]] const Value& raw(Keylike&& key) const
		{
			TR_ASSERT(valid(), "Tried to get entry in a dynamic atlas in an invalid state.");
			TR_ASSERT(contains(key), "Tried to get nonexistent entry in dynamic atlas {}.", *this);

			return m_entries[std::forward<Keylike>(key)];
		}

		/// @}

	  private:
		/// @brief Texture of the atlas.
		tr::texture m_texture;

		/// @brief Entires of the atlas.
		atlas_entries<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr

//

/// @brief Dynamic atlas formatter.
template <typename Key, tr::atlas_entries_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
struct std::formatter<tr::dynamic_atlas<Key, Value, Hash, Pred>>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a dynamic atlas.
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