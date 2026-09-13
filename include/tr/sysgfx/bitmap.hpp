/// @file
/// @brief Provides `tr::bitmap`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/sysgfx/pixel_iterator.hpp>
#include <tr/utility/exception.hpp>
#include <tr/utility/rectangle.hpp>

struct SDL_Surface;
namespace tr
{
	class bitmap_view;
}

//

namespace tr
{
	/// Error thrown when bitmap loading fails.
	class bitmap_load_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the bitmap file.
		/// @param details Details of the error.
		[[nodiscard]] bitmap_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap loading error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

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
		[[nodiscard]] bitmap_save_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Bitmap saving error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

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
		using reference = pixel_proxy;

		/// Constant reference type used by the bitmap.
		using const_reference = const_pixel_proxy;

		/// Iterator type used by the bitmap.
		using iterator = pixel_iterator;

		/// Constant iterator type used by the bitmap.
		using const_iterator = const_pixel_iterator;

		/// @name Constructors
		/// @{

		/// @cond sdl_interop

		/// Wraps an SDL surface pointer.
		/// @param ptr SDL surface pointer to wrap.
		[[nodiscard]] explicit bitmap(SDL_Surface* ptr);

		/// @endcond

		/// Creates a blank bitmap.
		/// @param size Size of the bitmap.
		/// @param format Pixel format of the bitmap.
		[[nodiscard]] explicit bitmap(glm::ivec2 size, pixel_format format = pixel_format::rgba32);

		/// Clones a bitmap.
		/// @param bitmap Source bitmap to copy.
		/// @param format Pixel format of the bitmap.
		[[nodiscard]] explicit bitmap(const bitmap& bitmap, pixel_format format = pixel_format::rgba32);

		/// Clones a bitmap view.
		/// @param view Source bitmap view to copy.
		/// @param format Pixel format of the bitmap.
		[[nodiscard]] explicit bitmap(const bitmap_view& view, pixel_format format = pixel_format::rgba32);

		/// Clones a sub-bitmap.
		/// @param source Source sub-bitmap to copy.
		/// @param format Pixel format of the bitmap.
		[[nodiscard]] explicit bitmap(sub_bitmap source, pixel_format format = pixel_format::rgba32);

		/// Moves a bitmap.
		/// @param bitmap Bitmap to move.
		[[nodiscard]] bitmap(bitmap&& bitmap) noexcept = default;

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
		[[nodiscard]] operator sub_bitmap() const noexcept;

		/// Creates a sub-bitmap of the bitmap.
		/// @param region Region of the bitmap to create a sub-bitmap of.
		/// @return Sub-bitmap spanning a region of the bitmap.
		[[nodiscard]] sub_bitmap sub(rectangle<int> region) const noexcept;

		/// @}
		/// @name Information
		/// @{

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

		/// Gets mutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](int x, int y) noexcept;

		/// Gets mutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] reference operator[](glm::ivec2 pos) noexcept;

		/// Gets immutable access to a pixel of the bitmap.
		/// @param x, y Position of the bitmap within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] const_reference operator[](int x, int y) const noexcept;

		/// Gets immutable access to a pixel of the bitmap.
		/// @param pos Position of the pixel within the bitmap.
		/// @return Reference to a pixel of the bitmap.
		[[nodiscard]] const_reference operator[](glm::ivec2 pos) const noexcept;

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		[[nodiscard]] std::byte* data() noexcept;

		/// Gets the raw data of the bitmap.
		/// @return Pointer to the data of the bitmap.
		[[nodiscard]] const std::byte* data() const noexcept;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets a mutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] iterator begin() noexcept;

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] const_iterator begin() const noexcept;

		/// Gets an immutable iterator to the beginning of the bitmap.
		/// @return Iterator to the beginning of the bitmap.
		[[nodiscard]] const_iterator cbegin() const noexcept;

		/// Gets a mutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] iterator end() noexcept;

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] const_iterator end() const noexcept;

		/// Gets an immutable iterator to one past the end of the bitmap.
		/// @return Iterator to the end of the bitmap.
		[[nodiscard]] const_iterator cend() const noexcept;

		/// @}
		/// @name Manipulation
		/// @{

		/// Blits a sub-bitmap to the bitmap.
		/// @param tl Top-left corner of the target region.
		/// @param source Source sub-bitmap to blit.
		void blit(glm::ivec2 tl, sub_bitmap source) noexcept;

		/// Fills a region of the bitmap with a solid color.
		/// @param region Region of the bitmap to fill.
		/// @param color Color to fill the region with.
		void fill(rectangle<int> region, rgba8 color) noexcept;

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
		[[nodiscard]] SDL_Surface* unwrap() const noexcept;

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

	/// @name Bitmap factories
	/// @{

	/// Creates a bitmap with the missing texture checkerboard pattern.
	/// @param size Size of the bitmap.
	/// @return Missing texture bitmap.
	[[nodiscard]] bitmap create_checkerboard(glm::ivec2 size);

	/// Loads an embedded bitmap file.
	/// @param data Bitmap data.
	/// @return Loaded bitmap.
	[[nodiscard]] bitmap load_embedded_bitmap(std::span<const std::byte> data);

	/// Loads an embedded bitmap file.
	/// @tparam Range Bitmap data range type.
	/// @param range Bitmap data range.
	/// @return Loaded bitmap.
	template <std::ranges::contiguous_range Range>
	[[nodiscard]] bitmap load_embedded_bitmap(Range&& range)
	{
		return load_embedded_bitmap(std::span<const std::byte>{range_bytes(range)});
	}

	/// Loads a bitmap from file (BMP/PNG/QOI).
	/// @param path Path to the bitmap file.
	/// @exception file_not_found If the bitmap was not found.
	/// @exception bitmap_load_error If the bitmap loading failed.
	/// @return Loaded bitmap.
	[[nodiscard]] bitmap load_bitmap_file(const std::filesystem::path& path);

	/// @}
} // namespace tr