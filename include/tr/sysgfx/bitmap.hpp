#pragma once
#include "../utility/color.hpp"
#include "../utility/exception.hpp"
#include "../utility/geometry.hpp"

struct SDL_Surface;

namespace tr {
	class bitmap;
	class bitmap_view;
	namespace system {
		class cursor;
		class ttfont;

		void set_window_icon(const bitmap& bitmap);
		void set_window_icon(const bitmap_view& view);
	}; // namespace system

	// Bitmap/texture pixel format.
	enum class pixel_format {
		R8 = 318769153,
		RGB_P332 = 336660481,
		XRGB_P4444 = 353504258,
		XBGR_P4444 = 357698562,
		XRGB_P1555 = 353570562,
		XBGR_P1555 = 357764866,
		ARGB_P4444 = 355602434,
		RGBA_P4444 = 356651010,
		ABGR_P4444 = 359796738,
		BGRA_P4444 = 360845314,
		ARGB_P1555 = 355667970,
		RGBA_P5551 = 356782082,
		ABGR_P1555 = 359862274,
		BGRA_P5551 = 360976386,
		RGB_P565 = 353701890,
		BGR_P565 = 357896194,
		RGB24 = 386930691,
		BGR24 = 390076419,
		BGRX32 = 370546692,
		XBGR32 = 371595268,
		RGBX32 = 374740996,
		XRGB32 = 375789572,
		BGRA32 = 372645892,
		ABGR32 = 373694468,
		RGBA32 = 376840196,
		ARGB32 = 377888772,
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
		friend class system::cursor;
		friend void system::set_window_icon(const bitmap_view& view);
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
		bitmap(glm::ivec2 size, pixel_format format = pixel_format::RGBA32);
		// Clones a bitmap.
		explicit bitmap(const bitmap& bitmap, pixel_format format = pixel_format::RGBA32);
		// Clones a bitmap view.
		explicit bitmap(const bitmap_view& view, pixel_format format = pixel_format::RGBA32);
		// Clones a sub-bitmap.
		explicit bitmap(const sub_bitmap& source, pixel_format format = pixel_format::RGBA32);
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

		bitmap(SDL_Surface* ptr);

		friend class sub_bitmap;
		friend class sub_bitmap::iterator;
		friend class mut_it;
		friend class system::cursor;
		friend class system::ttfont;
		friend void system::set_window_icon(const bitmap& bitmap);

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
