/// @file
/// @brief Provides a bitmap class and related datatypes.

#pragma once
#include "../utility/color.hpp"
#include "../utility/exception.hpp"
#include "bitmap_view.hpp"

//

namespace tr
{
	/// Bitmap/texture pixel format.
	enum class pixel_format
	{
		/// Single-channel color stored as an 8-bit integer.
		r8 = 318769153,
		/// Single-channel color stored as an 8-bit integer.
		index8 = r8,
		/// RGB color stored as an 8-bit integer.
		rgb_p332 = 336660481,
		/// XRGB color stored as a 16-bit integer (4-bit channels).
		xrgb_p4444 = 353504258,
		/// XBGR color stored as a 16-bit integer (4-bit channels).
		xbgr_p4444 = 357698562,
		/// XRGB color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		xrgb_p1555 = 353570562,
		/// XBGR color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		xbgr_p1555 = 357764866,
		/// ARGB color stored as a 16-bit integer (4-bit channels).
		argb_p4444 = 355602434,
		/// RGBA color stored as a 16-bit integer (4-bit channels).
		rgba_p4444 = 356651010,
		/// ABGR color stored as a 16-bit integer (4-bit channels).
		abgr_p4444 = 359796738,
		/// BGRA color stored as a 16-bit integer (4-bit channels).
		bgra_p4444 = 360845314,
		/// ARGB color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		argb_p1555 = 355667970,
		/// RGBA color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		rgba_p5551 = 356782082,
		/// ABGR color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		abgr_p1555 = 359862274,
		/// BGRA color stored as a 16-bit integer (5-bit color, 1-bit alpha).
		bgra_p5551 = 360976386,
		/// RGB color stored as a 16-bit integer.
		rgb_p565 = 353701890,
		/// BGR color stored as a 16-bit integer.
		bgr_p565 = 357896194,
		/// RGB color stored as an array.
		rgb24 = 386930691,
		/// BGR color stored as an array.
		bgr24 = 390076419,
		/// BGRX color stored as a 32-bit integer.
		bgrx32 = 370546692,
		/// XBGR color stored as a 32-bit integer.
		xbgr32 = 371595268,
		/// RGBX color stored as a 32-bit integer.
		rgbx32 = 374740996,
		/// XRGB color stored as a 32-bit integer.
		xrgb32 = 375789572,
		/// BGRA color stored as a 32-bit integer.
		bgra32 = 372645892,
		/// ABGR color stored as a 32-bit integer.
		abgr32 = 373694468,
		/// RGBA color stored as a 32-bit integer.
		rgba32 = 376840196,
		/// ARGB color stored as a 32-bit integer.
		argb32 = 377888772,
	};

	/// @name Pixel format
	/// @{

	/// Gets the number of bytes per pixel for a given format.
	/// @param format Pixel format type.
	/// @return Number of bytes per pixel for `format`.
	int pixel_bytes(pixel_format format);

	/// @}

	/// Error thrown when bitmap loading fails.
	class bitmap_load_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the bitmap file.
		/// @param details Details of the error.
		bitmap_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap loading error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Details of the error.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Error thrown when bitmap saving fails.
	class bitmap_save_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the bitmap file.
		/// @param details Details of the error.
		bitmap_save_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap saving error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Details of the error.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	//

	/// Class containing owned bitmap data.
	class bitmap
	{
	  public:
		/// Reference type used by the bitmap.
		class reference;

		/// Constant reference type used by the bitmap.
		using const_reference = sub_bitmap::reference;

		/// Iterator type used by the bitmap.
		class iterator;

		/// Constant iterator type used by the bitmap.
		using const_iterator = sub_bitmap::iterator;

		/// @name Constructors
		/// @{

		/// @cond sdl_interop

		/// Wraps an SDL surface pointer.
		/// @param ptr SDL surface pointer to wrap.
		explicit bitmap(SDL_Surface* ptr);

		/// @endcond

		/// Creates a blank bitmap.
		/// @param size Size of the bitmap.
		/// @param format Pixel format of the bitmap.
		explicit bitmap(glm::ivec2 size, pixel_format format = pixel_format::rgba32);

		/// Clones a bitmap.
		/// @param bitmap Source bitmap to copy.
		/// @param format Pixel format of the bitmap.
		explicit bitmap(const bitmap& bitmap, pixel_format format = pixel_format::rgba32);

		/// Clones a bitmap view.
		/// @param view Source bitmap view to copy.
		/// @param format Pixel format of the bitmap.
		explicit bitmap(const bitmap_view& view, pixel_format format = pixel_format::rgba32);

		/// Clones a sub-bitmap.
		/// @param source Source sub-bitmap to copy.
		/// @param format Pixel format of the bitmap.
		explicit bitmap(sub_bitmap source, pixel_format format = pixel_format::rgba32);

		/// Moves a bitmap.
		/// @param bitmap Bitmap to move.
		bitmap(bitmap&& bitmap) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Moves a bitmap.
		/// @param rhs Bitmap to move.
		/// @return Reference to `*this`.
		bitmap& operator=(bitmap&& rhs) noexcept = default;

		/// @}
		/// @name Sub-bitmap
		/// @{

		/// Creates a sub-bitmap spanning the entire bitmap.
		/// @return Sub-bitmap spanning the entire bitmap.
		operator sub_bitmap() const;

		/// Creates a sub-bitmap of the bitmap.
		/// @param region Region of the bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap spanning a region of the bitmap.
		sub_bitmap sub(rectangle<int> region) const;

		/// @}
		/// @name Information
		/// @{

		/// Gets the size of the bitmap.
		/// @return Size of the bitmap.
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

		/// Gets mutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		reference operator[](int x, int y);

		/// Gets mutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		reference operator[](glm::ivec2 pos);

		/// Gets immutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		const_reference operator[](int x, int y) const;

		/// Gets immutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		const_reference operator[](glm::ivec2 pos) const;

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		std::byte* data();

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		const std::byte* data() const;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets a mutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		iterator begin();

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		const_iterator begin() const;

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		const_iterator cbegin() const;

		/// Gets a mutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		iterator end();

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		const_iterator end() const;

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		const_iterator cend() const;

		/// @}
		/// @name Manipulation
		/// @{

		/// Blits a sub-bitmap to the bitmap.
		/// @param tl Top-left corner of the target region.
		/// @param source Source sub-bitmap to blit.
		void blit(glm::ivec2 tl, sub_bitmap source);

		/// Fills a region of the bitmap with a solid color.
		/// @param region Region of the bitmap to fill.
		/// @param color Color to fill the region with.
		void fill(rectangle<int> region, rgba8 color);

		/// @}
		/// @name Saving
		/// @{

		/// Saves the bitmap to a .png file.
		/// @exception bitmap_save_error If saving the bitmap to file failed.
		void save(const std::filesystem::path& path) const;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability

		/// Unwraps the SDL surface pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the SDL surface.
		SDL_Surface* unwrap() const;

		/// @endcond

	  private:
		/// Bitmap deleter.
		struct deleter
		{
			/// Destroys a bitmap.
			/// @param ptr Pointer to an SDL surface.
			static void operator()(SDL_Surface* ptr);
		};

		//

		/// Handle to the SDL surface.
		std::unique_ptr<SDL_Surface, deleter> m_ptr;
	};

	/// @name Bitmap factories
	/// @{

	/// Creates a bitmap with the missing texture checkerboard pattern.
	/// @param size Size of the bitmap.
	/// @return Missing texture bitmap.
	bitmap create_checkerboard(glm::ivec2 size);

	/// Loads an embedded bitmap file.
	/// @param data Bitmap data.
	/// @return Loaded bitmap.
	bitmap load_embedded_bitmap(std::span<const std::byte> data);

	/// Loads an embedded bitmap file.
	/// @tparam Range Bitmap data range type.
	/// @param range Bitmap data range.
	/// @return Loaded bitmap.
	template <std::ranges::contiguous_range Range>
	bitmap load_embedded_bitmap(Range&& range);

	/// Loads a bitmap from file (BMP/PNG/QOI).
	/// @param path Path to the bitmap file.
	/// @exception file_not_found If the bitmap was not found.
	/// @exception bitmap_load_error If the bitmap loading failed.
	/// @return Loaded bitmap.
	bitmap load_bitmap_file(const std::filesystem::path& path);

	/// @}
} // namespace tr

#include "impl/bitmap.hpp" // IWYU pragma: export