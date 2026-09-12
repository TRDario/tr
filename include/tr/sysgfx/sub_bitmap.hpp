/// @file
/// @brief Provides a sub-bitmap class.

#pragma once
#include "../utility/rectangle.hpp"

namespace tr
{
	class bitmap;
	class bitmap_view;
	enum class pixel_format;
} // namespace tr

struct SDL_Surface;

//

namespace tr
{
	/// View over a rectangular region of a bitmap.
	class sub_bitmap
	{
	  public:
		/// Immutable pixel reference.
		class reference;

		/// Immutable iterator.
		class iterator;

		/// @name Constructors
		/// @{

		/// Constructs a sub-bitmap.
		/// @param bitmap Bitmap to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(const bitmap& bitmap, rectangle<int> region) noexcept;

		/// Constructs a sub-bitmap.
		/// @param view Bitmap view to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(const bitmap_view& view, rectangle<int> region) noexcept;

		/// Constructs a sub-bitmap.
		/// @param sub Sub-bitmap to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(sub_bitmap sub, rectangle<int> region) noexcept;

		/// @}
		/// @name Sub-bitmaps
		/// @{

		/// Creates a sub-bitmap of the sub-bitmap.
		/// @param region Region of the sub-bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap of the sub-bitmap.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the region the sub-bitmap occupies within the parent bitmap.
		/// @return Region the sub-bitmap occupies within the parent bitmap.
		[[nodiscard]] rectangle<int> region() const noexcept;

		/// Gets the size of the sub-bitmap.
		/// @return Size of the sub-bitmap.
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

		/// Gets an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		[[nodiscard]] iterator begin() const noexcept;

		/// Gets an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		[[nodiscard]] iterator cbegin() const noexcept;

		/// Gets an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		[[nodiscard]] iterator end() const noexcept;

		/// Gets an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		[[nodiscard]] iterator cend() const noexcept;

		/// @}

		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL surface pointer.
		/// @return Pointer to the SDL surface.
		[[nodiscard]] SDL_Surface* unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Pointer to the bitmap.
		SDL_Surface* m_ptr;

		/// Region of the sub-bitmap within the bitmap.
		rectangle<int> m_region;
	};
} // namespace tr