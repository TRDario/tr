/// @file
/// @brief Provides `tr::bitmap_view`.

#pragma once
#include <tr/sysgfx/sub_bitmap.hpp>

namespace tr
{
	enum class pixel_format;
} // namespace tr

struct SDL_Surface;

//

namespace tr
{
	/// Non-owning bitmap view.
	/// @details
	/// `tr::bitmap_view` implements an interface analogous to `tr::bitmap`, but does not own or allow modification of the underlying pixel
	/// data. It is not to be confused with `tr::sub_bitmap`, which is a view into a sub-region of a bitmap or bitmap view.
	///
	/// Moved-from instances of `tr::bitmap_view` are left in a special 'invalid' state. Invalid `tr::bitmap_view` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()`.
	class bitmap_view
	{
	  public:
		/// Immutable pixel reference.
		using reference = const_pixel_proxy;

		/// Immutable iterator.
		using iterator = const_pixel_iterator;

		/// @name Constructors
		/// @{

		/// Creates a bitmap view over contiguous pixel data.
		/// @param raw_data Span holding the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		[[nodiscard]] bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format);

		/// Creates a bitmap view over a range of pixel data.
		/// @param range Range holding the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		template <std::ranges::contiguous_range Range>
		[[nodiscard]] bitmap_view(Range&& range, glm::ivec2 size, pixel_format format)
			: bitmap_view{std::span<const std::byte>{range_bytes(range)}, size, format}
		{
		}

		/// Creates a bitmap view over pixel data.
		/// @param raw_data_start Pointer to the pixel data.
		/// @param pitch Pitch of the pixel data.
		/// @param size Size of the bitmap.
		/// @param format Format of the bitmap.
		[[nodiscard]] bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format);

		/// Bitmap views are not copyable.
		bitmap_view(const bitmap_view&) = delete;

		/// Moves a bitmap view.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Bitmap view to move.
		[[nodiscard]] bitmap_view(bitmap_view&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Bitmap views are not copyable.
		bitmap_view& operator=(const bitmap_view&) = delete;

		/// Moves a bitmap view.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Bitmap view to move.
		/// @return Reference to `*this`.
		bitmap_view& operator=(bitmap_view&&) noexcept = default;

		/// @}
		/// @name Sub-bitmaps
		/// @{

		/// Creates a sub-bitmap spanning the entire bitmap view.
		/// @return Sub-bitmap spanning the entire bitmap view.
		[[nodiscard]] operator sub_bitmap() const noexcept;

		/// Creates a sub-bitmap of the bitmap view.
		/// @param region Region of the bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap spanning a region of the bitmap view.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) const noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets whether the bitmap view is valid.
		/// @return `true` if the bitmap view is valid, `false` otherwise.
		[[nodiscard]] bool valid() const noexcept;

		/// Gets the size of the bitmap.
		/// @return Size of the bitmap.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// Gets the format of the bitmap.
		/// @return Format of the bitmap.
		[[nodiscard]] pixel_format format() const noexcept;

		/// Gets the pitch of the bitmap.
		/// @return Pitch of the bitmap.
		[[nodiscard]] int pitch() const noexcept;

		/// @}
		/// @name Access
		/// @{

		/// Gets immutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](int x, int y) const noexcept;

		/// Gets immutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](glm::ivec2 pos) const noexcept;

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator begin() const noexcept;

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator cbegin() const noexcept;

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator end() const noexcept;

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator cend() const noexcept;

		/// @}
		/// @name Saving
		/// @{

		/// Saves the bitmap to a .png file.
		/// @exception bitmap_save_error If saving the bitmap to file failed.
		void save(const std::filesystem::path& path) const;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL surface pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the SDL surface.
		[[nodiscard]] SDL_Surface* unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Bitmap deleter.
		struct deleter
		{
			/// Destroys a bitmap.
			/// @param ptr Pointer to an SDL surface.
			static void operator()(SDL_Surface* ptr) noexcept;
		};

		//

		/// Handle to the SDL surface.
		std::unique_ptr<SDL_Surface, deleter> m_ptr;
	};
} // namespace tr