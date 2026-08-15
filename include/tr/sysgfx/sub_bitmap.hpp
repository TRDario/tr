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
		sub_bitmap(const bitmap& bitmap, rectangle<int> region);

		/// Constructs a sub-bitmap.
		/// @param view Bitmap view to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		sub_bitmap(const bitmap_view& view, rectangle<int> region);

		/// Constructs a sub-bitmap.
		/// @param sub Sub-bitmap to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		sub_bitmap(sub_bitmap sub, rectangle<int> region);

		/// @}
		/// @name Sub-bitmaps
		/// @{

		/// Creates a sub-bitmap of the sub-bitmap.
		/// @param region Region of the sub-bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap of the sub-bitmap.
		sub_bitmap sub(rectangle<int> region);

		/// @}
		/// @name Information
		/// @{

		/// Gets the region the sub-bitmap occupies within the parent bitmap.
		/// @return Region the sub-bitmap occupies within the parent bitmap.
		rectangle<int> region() const;

		/// Gets the size of the sub-bitmap.
		/// @return Size of the sub-bitmap.
		glm::ivec2 size() const;

		/// Gets the format of the bitmap.
		/// @return Format of the bitmap.
		pixel_format format() const;

		/// Gets the pitch of the bitmap.
		/// @return Pitch of the bitmap.
		int pitch() const;

		/// @}
		/// @name Access
		/// @{

		/// Gets immutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		reference operator[](int x, int y) const;

		/// Gets immutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		reference operator[](glm::ivec2 pos) const;

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		const std::byte* data() const;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		iterator begin() const;

		/// Gets an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		iterator cbegin() const;

		/// Gets an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		iterator end() const;

		/// Gets an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		iterator cend() const;

		/// @}

		/// @cond sdl_interop
		/// Unwraps the SDL surface pointer.
		/// @return Pointer to the SDL surface.
		SDL_Surface* unwrap() const;
		/// @endcond

	  private:
		/// Pointer to the bitmap.
		SDL_Surface* m_ptr;

		/// Region of the sub-bitmap within the bitmap.
		rectangle<int> m_region;
	};
} // namespace tr