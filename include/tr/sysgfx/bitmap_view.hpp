/// @file
/// @brief Provides `tr::bitmap_view`.

#pragma once
#include <tr/sysgfx/internal/sdl_surface_deleter.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>

//

namespace tr
{
	/// @brief Owning container for a two-dimensional bitmap image which does not own its pixel data.
	/// @details
	/// `tr::bitmap_view` implements an interface analogous to `tr::bitmap`, but does not own or allow modification of the underlying pixel
	/// data. It is not to be confused with `tr::sub_bitmap`, which is a view into a sub-region of a bitmap or bitmap view.
	///
	/// Moved-from instances of `tr::bitmap_view` are left in a special 'invalid' state. Invalid `tr::bitmap_view` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	class bitmap_view
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Reference type used by bitmap views.
		using reference = const_pixel_proxy;

		/// @brief Constant iterator type used by bitmap views.
		using iterator = const_pixel_iterator;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a bitmap view over contiguous pixel data.
		/// @param raw_data Span holding the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @post The data pointed to by `raw_data` must stay alive for the duration of the lifetime of the bitmap view.
		[[nodiscard]] bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format);

		/// @brief Constructs a bitmap view over a range of pixel data.
		/// @param range Range holding the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @post The data pointed to by `range` must stay alive for the duration of the lifetime of the bitmap view.
		template <std::ranges::contiguous_range Range>
			requires std::ranges::borrowed_range<Range>
		[[nodiscard]] bitmap_view(Range&& range, glm::ivec2 size, pixel_format format)
			: bitmap_view{std::span<const std::byte>{range_bytes(range)}, size, format}
		{
		}

		/// @brief Constructs a bitmap view over pixel data.
		/// @param raw_data_start Pointer to the pixel data.
		/// @param pitch Pitch of the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @post The data pointed to by `raw_data_start` must stay alive for the duration of the lifetime of the bitmap view.
		[[nodiscard]] bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format);

		/// @brief Bitmap views are not copyable.
		bitmap_view(const bitmap_view&) = delete;

		/// @brief Bitmap views are not copyable.
		bitmap_view& operator=(const bitmap_view&) = delete;

		/// @brief Constructs a bitmap view by transferring ownership from another bitmap view.
		/// @param rhs Bitmap view to move from.
		/// @par Invalidation
		/// All iterators and sub-bitmaps previously associated with `rhs` stay valid, but now point to `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] bitmap_view(bitmap_view&& rhs) noexcept = default;

		/// @brief Destroys the currenly held bitmap view and replaces it with one moved from another bitmap view object.
		/// @param rhs Bitmap view to move from.
		/// @par Invalidation
		/// All iterators and sub-bitmaps previously associated with `*this` are invalidated.
		///
		/// All iterators and sub-bitmaps previously associated with `rhs` stay valid, but now point to `*this`.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		bitmap_view& operator=(bitmap_view&& rhs) noexcept = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns an immutable proxy to a pixel of the bitmap.
		/// @param x, y Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](int x, int y) const noexcept;

		/// @brief Returns an immutable proxy to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](glm::ivec2 pos) const noexcept;

		/// @brief Returns a pointer to the raw data of the bitmap.
		/// @return Pointer to the raw data of the bitmap.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator begin() const noexcept;

		/// @brief Returns an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator cbegin() const noexcept;

		/// @brief Returns an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator end() const noexcept;

		/// @brief Returns an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator cend() const noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the bitmap view is in a valid state.
		/// @return `true` if the bitmap view is in a valid state, `false` otherwise.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the bitmap view is in a valid state.
		/// @return `true` if the bitmap view is in a valid state, `false` otherwise.
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

		/// @brief Returns a sub-bitmap spanning the entire bitmap view.
		/// @return Sub-bitmap spanning the entire bitmap view.
		[[nodiscard]] operator sub_bitmap() const noexcept;

		/// @cond sdl_interop
		/// @brief Returns a pointer to the underlying SDL surface.
		/// @return Pointer to the underlying SDL surface.
		[[nodiscard]] SDL_Surface* pointer() const noexcept;
		/// @endcond

		/// @}
		/// @name Operations
		/// @{

		/// @brief Returns a sub-bitmap of the bitmap view.
		/// @param region Region of the bitmap view to create a sub-bitmap of.
		/// @return Sub-bitmap spanning a region of the bitmap view.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) const noexcept;

		/// @brief Saves the bitmap view to a .png file.
		/// @param path Path to the bitmap file.
		/// @exception bitmap_save_error If saving the bitmap view to file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void save(const std::filesystem::path& path) const;

		/// @}

	  private:
		/// @brief Managed pointer to an SDL surface.
		std::unique_ptr<SDL_Surface, internal::sdl_surface_deleter> m_ptr;
	};
} // namespace tr