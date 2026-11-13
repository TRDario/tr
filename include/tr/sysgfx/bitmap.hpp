/// @file
/// @brief Provides `tr::bitmap`.

#pragma once
#include <tr/sysgfx/internal/sdl_surface_deleter.hpp>
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/integer.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/rgba.hpp>
#include <tr/utility/zstring_view.hpp>

namespace tr
{
	class bitmap_view;
	class const_pixel_iterator;
	class const_pixel_proxy;
	class pixel_iterator;
	class pixel_proxy;
	class sub_bitmap;
} // namespace tr

//

namespace tr
{
	/// @brief Owning container for a two-dimensional bitmap image.
	/// @details
	/// Moved-from instances of `tr::bitmap` are left in a special 'invalid' state. Invalid `tr::bitmap` instances may not be interacted
	/// with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	class bitmap
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Reference type used by the bitmap.
		using reference = pixel_proxy;

		/// @brief Constant reference type used by the bitmap.
		using const_reference = const_pixel_proxy;

		/// @brief Iterator type used by the bitmap.
		using iterator = pixel_iterator;

		/// @brief Constant iterator type used by the bitmap.
		using const_iterator = const_pixel_iterator;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @cond sdl_interop

		/// @brief Constructs a bitmap by taking ownership of an SDL surface pointer.
		/// @param ptr SDL surface pointer to take ownership of.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit bitmap(SDL_Surface* ptr);

		/// @endcond

		/// @brief Constructs a blank bitmap.
		/// @param size Size of the bitmap.
		/// @param format Pixel format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit bitmap(glm::ivec2 size, pixel_format format = pixel_format::rgba32);

		/// @brief Constructs a bitmap by cloning another bitmap.
		/// @param bitmap Source bitmap to copy from.
		/// @param format Pixel format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit bitmap(const bitmap& bitmap, pixel_format format = pixel_format::rgba32);

		/// @brief Constructs a bitmap by cloning a bitmap view.
		/// @param view Source bitmap view to copy from.
		/// @param format Pixel format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit bitmap(const bitmap_view& view, pixel_format format = pixel_format::rgba32);

		/// @brief Constructs a bitmap by cloning a sub-bitmap.
		/// @param source Source sub-bitmap to copy from.
		/// @param format Pixel format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit bitmap(sub_bitmap source, pixel_format format = pixel_format::rgba32);

		/// @brief Bitmaps are not copyable.
		bitmap(const bitmap&) = delete;

		/// @brief Bitmaps are not copyable.
		bitmap& operator=(const bitmap&) = delete;

		/// @brief Constructs a bitmap by transferring ownership from another bitmap.
		/// @param rhs Bitmap to move from.
		/// @par Invalidation
		/// All iterators and sub-bitmaps previously associated with `rhs` stay valid, but now point to `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] bitmap(bitmap&& rhs) noexcept = default;

		/// @brief Destroys the currenly held bitmap and replaces it with one moved from another bitmap object.
		/// @param rhs Bitmap to move from.
		/// @par Invalidation
		/// All iterators and sub-bitmaps previously associated with `*this` are invalidated.
		///
		/// All iterators and sub-bitmaps previously associated with `rhs` stay valid, but now point to `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		bitmap& operator=(bitmap&& rhs) noexcept = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a mutable proxy to a pixel of the bitmap.
		/// @param x, y Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](int x, int y) noexcept;

		/// @brief Returns a mutable proxy to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](glm::ivec2 pos) noexcept;

		/// @brief Returns an immutable proxy to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] const_reference operator[](int x, int y) const noexcept;

		/// @brief Returns an immutable proxy to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] const_reference operator[](glm::ivec2 pos) const noexcept;

		/// @brief Returns a pointer to the raw data of the bitmap.
		/// @return Pointer to the raw data of the bitmap.
		[[nodiscard]] std::byte* data() noexcept;

		/// @brief Returns a pointer to the raw data of the bitmap.
		/// @return Pointer to the raw data of the bitmap.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// @brief Returns a sub-bitmap spanning the entire bitmap.
		/// @return Sub-bitmap spanning the entire bitmap.
		[[nodiscard]] operator sub_bitmap() const noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns a mutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator begin() noexcept;

		/// @brief Returns an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] const_iterator begin() const noexcept;

		/// @brief Returns an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] const_iterator cbegin() const noexcept;

		/// @brief Returns a mutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator end() noexcept;

		/// @brief Returns an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] const_iterator end() const noexcept;

		/// @brief Returns an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] const_iterator cend() const noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Blits a sub-bitmap to the bitmap.
		/// @param tl Top-left corner of the target region.
		/// @param source Source sub-bitmap to blit.
		void blit(glm::ivec2 tl, sub_bitmap source) noexcept;

		/// @brief Fills a region of the bitmap with a solid color.
		/// @param region Region of the bitmap to fill.
		/// @param color Color to fill the region with.
		void fill(rectangle<int> region, rgba<u8> color) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the bitmap is in a valid state.
		/// @return `true` if the bitmap is in a valid state, `false` otherwise.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the bitmap is in a valid state.
		/// @return `true` if the bitmap is in a valid state, `false` otherwise.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns the size of the bitmap.
		/// @return Size of the bitmap.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @brief Returns the format of the bitmap.
		/// @return Format of the bitmap.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @brief Returns the pitch of the bitmap.
		/// @return Pitch of the bitmap.
		[[nodiscard]] int pitch() const noexcept;

		/// @cond sdl_interop
		/// @brief Returns a pointer to the underlying SDL surface.
		/// @return Pointer to the underlying SDL surface.
		[[nodiscard]] SDL_Surface* pointer() const noexcept;
		/// @endcond

		/// @}
		/// @name Operations
		/// @{

		/// @brief Returns a sub-bitmap of the bitmap.
		/// @param region Region of the bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap spanning a region of the bitmap.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) const noexcept;

		/// @brief Saves the bitmap to a .png file.
		/// @param path Path to the bitmap file.
		/// @exception bitmap_save_error If saving the bitmap to file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void save(const std::filesystem::path& path) const;

		/// @}

	  private:
		/// @brief Managed pointer to an SDL surface.
		std::unique_ptr<SDL_Surface, internal::sdl_surface_deleter> m_ptr;
	};

	//

	/// @brief Constructs a bitmap with the missing texture checkerboard pattern.
	/// @param size Size of the bitmap.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Missing texture bitmap.
	[[nodiscard]] bitmap create_checkerboard(glm::ivec2 size);

	/// @brief Loads an embedded bitmap file.
	/// @param data Bitmap data.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded bitmap.
	[[nodiscard]] bitmap load_embedded_bitmap(std::span<const std::byte> data);

	/// @brief Loads an embedded bitmap file.
	/// @tparam Range Bitmap data range type.
	/// @param range Bitmap data range.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded bitmap.
	template <std::ranges::contiguous_range Range>
	[[nodiscard]] bitmap load_embedded_bitmap(Range&& range)
	{
		return load_embedded_bitmap(std::span<const std::byte>{range_bytes(range)});
	}

	/// @brief Loads a bitmap from file (BMP/PNG/QOI).
	/// @param path Path to the bitmap file.
	/// @exception file_not_found If the bitmap was not found.
	/// @exception bitmap_load_error If the bitmap loading failed.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Loaded bitmap.
	[[nodiscard]] bitmap load_bitmap_file(const std::filesystem::path& path);
} // namespace tr