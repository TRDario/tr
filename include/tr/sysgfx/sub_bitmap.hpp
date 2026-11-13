/// @file
/// @brief Provides `tr::sub_bitmap`.

#pragma once
#include <tr/utility/rectangle.hpp>

namespace tr
{
	class bitmap;
	class bitmap_view;
	class const_pixel_iterator;
	class const_pixel_proxy;
	enum class pixel_format;
} // namespace tr

struct SDL_Surface;

//

namespace tr
{
	/// @brief View over a rectangular region of a bitmap.
	/// @details
	/// `tr::sub_bitmap` is a lightweight reference to a region of a `tr::bitmap` or `tr::bitmap_view` object. It is designed to be cheap to
	/// pass around and copy.
	///
	/// A `tr::sub_bitmap` should not outlive the bitmap or bitmap view it is pointing at, as it will otherwise become
	/// dangling. Instances of `tr::sub_bitmap` always anchor themselves relative to the top-level bitmap or bitmap view, so you do not have
	/// to worry about the lifetime of intermediate sub-bitmaps used to construct other sub-bitmaps.
	class sub_bitmap
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Immutable pixel reference used by the sub-bitmap.
		using reference = const_pixel_proxy;

		/// @brief Immutable iterator used by the sub-bitmap.
		using iterator = const_pixel_iterator;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a sub-bitmap.
		/// @param bitmap Bitmap to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(const bitmap& bitmap, rectangle<int> region) noexcept;

		/// @brief Constructs a sub-bitmap.
		/// @param view Bitmap view to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(const bitmap_view& view, rectangle<int> region) noexcept;

		/// @brief Constructs a sub-bitmap.
		/// @param sub Sub-bitmap to create a sub-bitmap on.
		/// @param region Sub-bitmap region.
		[[nodiscard]] sub_bitmap(sub_bitmap sub, rectangle<int> region) noexcept;

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

		/// @brief Returns an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		[[nodiscard]] iterator begin() const noexcept;

		/// @brief Returns an immutable iterator to the beginning of the sub-bitmap.
		/// @return Iterator to the beginning of the sub-bitmap.
		[[nodiscard]] iterator cbegin() const noexcept;

		/// @brief Returns an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		[[nodiscard]] iterator end() const noexcept;

		/// @brief Returns an immutable iterator to one past the end of the sub-bitmap.
		/// @return Iterator to the end of the sub-bitmap.
		[[nodiscard]] iterator cend() const noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the region the sub-bitmap occupies within the parent bitmap.
		/// @return Region the sub-bitmap occupies within the parent bitmap.
		[[nodiscard]] rectangle<int> region() const noexcept;

		/// @brief Returns the size of the sub-bitmap.
		/// @return Size of the sub-bitmap.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @brief Returns the format of the bitmap.
		/// @return Format of the bitmap.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @brief Returns the pitch of the bitmap.
		/// @return Pitch of the bitmap.
		[[nodiscard]] int pitch() const noexcept;

		/// @}
		/// @name Operations
		/// @{

		/// @brief Returns a sub-bitmap of the sub-bitmap.
		/// @param region Region of the sub-bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap of the sub-bitmap.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) noexcept;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// @brief Returns the underlying SDL surface pointer.
		/// @return Underlying SDL surface pointer.
		[[nodiscard]] SDL_Surface* pointer() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// @brief Underlying SDL surface pointer.
		SDL_Surface* m_ptr;

		/// @brief Region of the sub-bitmap within the bitmap.
		rectangle<int> m_region;
	};
} // namespace tr