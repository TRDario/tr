///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides bitmap classes and related functionality.                                                                                    //
//                                                                                                                                       //
// Bitmaps are collections of owned pixel data representing 2D images. Bitmaps can be constructed uninitialized (with a specified size), //
// cloned from another bitmap or sub-bitmap, loaded from embedded data or a file (BMP/PNG/JPG/QOI), or created with a checkerboard       //
// pattern (for missing textures, for example). Bitmaps may store their pixels in one of a number of pixel formats, which may be set     //
// during construction and queried with the .pixel_format() method. The size of bitmaps cannot be changed after construction and can be  //
// queried with the .size() method.                                                                                                      //
//     - tr::bitmap bitmap{512, 512} -> creates an uninitialized rgba32 bitmap of size 512x512                                           //
//     - bitmap.size() -> {512, 512}                                                                                                     //
//     - tr::bitmap bitmap{another_bitmap, tr::pixel_format::rgb24} -> clones another bitmap and converts it to rgb24                    //
//     - bitmap.pixel_format() -> tr::pixel_format::rgb24                                                                                //
//     - tr::bitmap{tr::create_checkerboard({32, 32})} -> creates a 32x32 bitmap with a checkerboard pattern                             //
//     - tr::bitmap{tr::load_embedded_bitmap(embedded_qoi)} -> loads an embedded QOI file                                                //
//     - tr::bitmap{tr::load_bitmap_file("bitmap.png")} -> loads a bitmap from bitmap.png                                                //
//                                                                                                                                       //
// Pixels of a bitmap may be accessed via a 2D index. In addition, bitmaps may be iterated through (for the details of the iterators and //
// pixel proxies see bitmap_iterators.hpp), or have their data directly taken via .data(). Note that the data in a bitmap isn't required //
// to be contiguous, bitmap.size().x * tr::pixel_bytes(bitmap.pixel_format()) != bitmap.pitch() (the actual length of a row in bytes) in //
// some cases:                                                                                                                           //
//     - bitmap[{50, 50}] = "FFFFFF"_rgba8 -> sets the pixel at (50, 50) to white                                                        //
//     - for (tr::pixel_ref p : bitmap) { p = tr::rgb8{p} * 0.75f } -> reduces the brightness of every pixel by 25%                      //
//     - &*(bitmap.begin() + 50) == bitmap.data() + bitmap.pitch() -> true                                                               //
//                                                                                                                                       //
// A bitmap or a region of another bitmap may be blitted onto a bitmap with the .blit() method, and the bitmap may be cleared to a       //
// single color using the .fill() method:                                                                                                //
//     - bitmap.blit({25, 25}, other) -> blits 'other' to bitmap with the top-left corner at (25, 25)                                    //
//     - bitmap.fill("FF00FF"_rgba8) -> fills the bitmap with magenta                                                                    //
//                                                                                                                                       //
// Bitmaps may be saved to a .png file using the .save() method:                                                                         //
//     - bitmap.save("bitmap.png") -> the contents of bitmap are saved to bitmap.png                                                     //
//                                                                                                                                       //
// Bitmap views share the interface of bitmaps, but do not own their data and are read-only:                                             //
//     - tr::bitmap_view view{embedded_bitmap_data, {256, 256}, tr::pixel_format::rgba32}                                                //
//       -> creates a 256x256 view over embedded contiguous raw bitmap data                                                              //
//     - tr::bitmap_view view{embedded_bitmap_data, 800, {256, 256}, tr::pixel_format::rgb24}                                            //
//       -> creates a 256x256 view over embedded raw bitmap data with a pitch of 800 bytes                                               //
//                                                                                                                                       //
// Sub-bitmaps are views over a region of a bitmap, bitmap view, or another sub-bitmap. They share most of their interface with bitmaps, //
// but cannot be saved to file or passed to certain tr::sys functions. Like bitmap views, they are read-only. Bitmaps and bitmap views   //
// can be implicitly converted to sub-bitmaps, and any of the three source types can create sub-bitmaps with the .sub() method:          //
//     - tr::sub_bitmap{bitmap}                                                                                                          //
//       -> creates a sub-bitmap over an entire bitmap                                                                                   //
//     - tr::sub_bitmap{bitmap, {{128, 128}, {128, 128}}}                                                                                //
//       -> creates a sub-bitmap over 'bitmap' with the top-left corner at (128, 128) and of size 128x128                                //
//     - bitmap.sub({{128, 128}, {128, 128}})                                                                                            //
//       -> equivalent to the above                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/color.hpp"
#include "../utility/exception.hpp"
#include "../utility/geometry.hpp"

struct SDL_Surface;
namespace tr {
	class bitmap;
	class bitmap_view;
	namespace sys {
		class cursor;
		class ttfont;

		void set_window_icon(const bitmap& bitmap);
		void set_window_icon(const bitmap_view& view);
	}; // namespace sys
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Bitmap/texture pixel format.
	enum class pixel_format {
		r8 = 318769153,
		rgb_p332 = 336660481,
		xrgb_p4444 = 353504258,
		xbgr_p4444 = 357698562,
		xrgb_p1555 = 353570562,
		xbgr_p1555 = 357764866,
		argb_p4444 = 355602434,
		rgba_p4444 = 356651010,
		abgr_p4444 = 359796738,
		bgra_p4444 = 360845314,
		argb_p1555 = 355667970,
		rgba_p5551 = 356782082,
		abgr_p1555 = 359862274,
		bgra_p5551 = 360976386,
		rgb_p565 = 353701890,
		bgr_p565 = 357896194,
		rgb24 = 386930691,
		bgr24 = 390076419,
		bgrx32 = 370546692,
		xbgr32 = 371595268,
		rgbx32 = 374740996,
		xrgb32 = 375789572,
		bgra32 = 372645892,
		abgr32 = 373694468,
		rgba32 = 376840196,
		argb32 = 377888772,
	};
	// Gets the number of bytes per pixel for a given format.
	int pixel_bytes(pixel_format format);

	// Error thrown when bitmap loading fails.
	class bitmap_load_error : public exception {
	  public:
		// Constructs an exception.
		bitmap_load_error(std::string_view path, std::string&& details);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
		// The details of the error.
		std::string m_details;
	};
	// Error thrown when bitmap saving fails.
	class bitmap_save_error : public exception {
	  public:
		// Constructs an exception.
		bitmap_save_error(std::string_view path, std::string&& details);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
		// The details of the error.
		std::string m_details;
	};

	// View over a rectangular region of a bitmap.
	class sub_bitmap {
	  public:
		class pixel_ref;
		class iterator;

		// Constructs a sub-bitmap.
		sub_bitmap(const bitmap& bitmap, const irect2& rect);
		// Constructs a sub-bitmap.
		sub_bitmap(const bitmap_view& view, const irect2& rect);

		// Gets the size of the sub-bitmap.
		glm::ivec2 size() const;

		// Creates a sub-bitmap of the sub-bitmap.
		sub_bitmap sub(const irect2& rect);

		// Gets immutable access to a pixel of the bitmap.
		pixel_ref operator[](glm::ivec2 pos) const;

		// Gets an immutable iterator to the beginning of the sub-bitmap.
		iterator begin() const;
		// Gets an immutable iterator to the beginning of the sub-bitmap.
		iterator cbegin() const;
		// Gets an immutable iterator to one past the end of the sub-bitmap.
		iterator end() const;
		// Gets an immutable iterator to one past the end of the sub-bitmap.
		iterator cend() const;

		// Gets the raw data of the sub-bitmap.
		const std::byte* data() const;

		// Gets the format of the bitmap.
		pixel_format format() const;
		// Gets the pitch of the bitmap.
		int pitch() const;

	  private:
		// Pointer to the bitmap.
		SDL_Surface* m_ptr;
		// The rect of the sub-bitmap within the bitmap.
		irect2 m_rect;

		friend class bitmap;
	};

	// Non-owning view over bitmap data.
	class bitmap_view {
	  public:
		// Immutable pixel reference.
		using pixel_ref = sub_bitmap::pixel_ref;
		// Immutable iterator.
		using iterator = sub_bitmap::iterator;

		// Creates a bitmap view over contiguous pixel data.
		bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format);
		// Creates a bitmap view over a range of pixel data.
		template <std::ranges::contiguous_range T> bitmap_view(T&& range, glm::ivec2 size, pixel_format format);
		// Creates a bitmap view over pixel data.
		bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format);

		// Gets the size of the bitmap.
		glm::ivec2 size() const;

		// Gets immutable access to a pixel of the bitmap.
		pixel_ref operator[](glm::ivec2 pos) const;

		// Gets an immutable iterator to the beginning of the bitmap.
		iterator begin() const;
		// Gets an immutable iterator to the beginning of the bitmap.
		iterator cbegin() const;
		// Gets an immutable iterator to one past the end of the bitmap.
		iterator end() const;
		// Gets an immutable iterator to one past the end of the bitmap.
		iterator cend() const;

		// Creates a sub-bitmap spanning the entire bitmap view.
		operator sub_bitmap() const;
		// Creates a sub-bitmap of the bitmap.
		sub_bitmap sub(const irect2& rect) const;

		// Gets the raw data of the bitmap.
		const std::byte* data() const;

		// Gets the format of the bitmap.
		pixel_format format() const;
		// Gets the pitch of the bitmap.
		int pitch() const;

		// Saves the bitmap to a .png file.
		// May throw: bitmap_save_error.
		void save(const std::filesystem::path& path) const;

	  private:
		struct deleter {
			void operator()(SDL_Surface* ptr) const;
		};

		// Handle to the SDL surface.
		std::unique_ptr<SDL_Surface, deleter> m_ptr;

		friend class bitmap;
		friend class sub_bitmap;
		friend class sys::cursor;
		friend void sys::set_window_icon(const bitmap_view& view);
	};

	// Class containing owned bitmap data.
	class bitmap {
	  public:
		class pixel_ref;
		class mut_it;

		// Immutable pixel reference.
		using pixel_cref = tr::sub_bitmap::pixel_ref;
		// Immutable iterator.
		using const_it = tr::sub_bitmap::iterator;

		// Creates a blank bitmap.
		bitmap(glm::ivec2 size, pixel_format format = pixel_format::rgba32);
		// Clones a bitmap.
		explicit bitmap(const bitmap& bitmap, pixel_format format = pixel_format::rgba32);
		// Clones a bitmap view.
		explicit bitmap(const bitmap_view& view, pixel_format format = pixel_format::rgba32);
		// Clones a sub-bitmap.
		explicit bitmap(const sub_bitmap& source, pixel_format format = pixel_format::rgba32);
		bitmap(bitmap&& bitmap) noexcept = default;

		bitmap& operator=(bitmap&& r) noexcept = default;

		// Gets the size of the bitmap.
		glm::ivec2 size() const;

		// Gets mutable access to a pixel of the bitmap.
		pixel_ref operator[](glm::ivec2 pos);
		// Gets immutable access to a pixel of the bitmap.
		pixel_cref operator[](glm::ivec2 pos) const;

		// Gets a mutable iterator to the beginning of the bitmap.
		mut_it begin();
		// Gets an immutable iterator to the beginning of the bitmap.
		const_it begin() const;
		// Gets an immutable iterator to the beginning of the bitmap.
		const_it cbegin() const;
		// Gets a mutable iterator to one past the end of the bitmap.
		mut_it end();
		// Gets an immutable iterator to one past the end of the bitmap.
		const_it end() const;
		// Gets an immutable iterator to one past the end of the bitmap.
		const_it cend() const;

		// Blits a sub-bitmap to the bitmap.
		void blit(glm::ivec2 tl, const sub_bitmap& source);
		// Fills a region of the bitmap with a solid color.
		void fill(const irect2& rect, rgba8 color);

		// Creates a sub-bitmap spanning the entire bitmap.
		operator sub_bitmap() const;
		// Creates a sub-bitmap of the bitmap.
		sub_bitmap sub(const irect2& rect) const;

		// Gets the raw data of the bitmap.
		std::byte* data();
		// Gets the raw data of the bitmap.
		const std::byte* data() const;

		// Gets the format of the bitmap.
		pixel_format format() const;
		// Gets the pitch of the bitmap.
		int pitch() const;

		// Saves the bitmap to a .png file.
		// May throw: bitmap_save_error.
		void save(const std::filesystem::path& path) const;

	  private:
		// Handle to the SDL surface.
		std::unique_ptr<SDL_Surface, bitmap_view::deleter> m_ptr;

		// Wraps an SDL surface pointer.
		bitmap(SDL_Surface* ptr);

		friend class sub_bitmap;
		friend class sub_bitmap::iterator;
		friend class mut_it;
		friend class sys::cursor;
		friend class sys::ttfont;
		friend void sys::set_window_icon(const bitmap& bitmap);

		friend bitmap load_embedded_bitmap(std::span<const std::byte> data);
		friend bitmap load_bitmap_file(const std::filesystem::path& path);
	};
	// Creates a bitmap with the missing texture checkerboard pattern.
	bitmap create_checkerboard(glm::ivec2 size);
	// Loads an embedded bitmap file.
	bitmap load_embedded_bitmap(std::span<const std::byte> data);
	// Loads an embedded bitmap file.
	template <std::ranges::contiguous_range R> bitmap load_embedded_bitmap(R&& range);
	// Loads a bitmap from file (BMP/PNG/JPG/QOI).
	// May throw: file_not_found, bitmap_load_error.
	bitmap load_bitmap_file(const std::filesystem::path& path);
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range T>
tr::bitmap_view::bitmap_view(T&& range, glm::ivec2 size, pixel_format format)
	: bitmap_view{std::span<const std::byte>{range_bytes(range)}, size, format}
{
}

template <std::ranges::contiguous_range R> tr::bitmap tr::load_embedded_bitmap(R&& range)
{
	return load_embedded_bitmap(std::span<const std::byte>{range_bytes(range)});
}
