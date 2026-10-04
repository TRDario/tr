/// @file
/// @brief Provides `tr::texture`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/color.hpp>
#include <tr/utility/handle.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class graphics_context;
	class mutable_texture_view;
	class sub_bitmap;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Whether mipmapping should be enabled or disabled on a texture.
	enum class mipmaps : bool
	{
		/// Mipmapping disabled.
		disabled,

		/// Mipmapping enabled.
		enabled
	};

	/// Texture wrapping types.
	enum class wrap
	{
		/// The texture is repeated.
		repeat = 0x2901,

		/// The texture is repeated and mirrored.
		mirror_repeat = 0x8370,

		/// The value of the edge pixel is used.
		edge_clamp = 0x812F,

		/// The value of the border color is used.
		border_clamp = 0x812D
	};

	/// Minifying filter types.
	enum class min_filter
	{
		/// The value of the texture element that is nearest to the specified texture coordinates is used.
		nearest = 0x2600,

		/// The average of the four texture elements that are closest to the specified texture coordinates is used.
		linear = 0x2601,

		/// Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		nmip_nearest = 0x2700,

		/// Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		nmip_linear = 0x2702,

		/// Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		lmips_nearest = 0x2701,

		/// Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		lmips_linear = 0x2703
	};

	/// Magnifying filter types.
	enum class mag_filter
	{
		/// The value of the texture element that is nearest to the specified texture coordinates is used.
		nearest = 0x2600,

		/// The average of the four texture elements that are closest to the specified texture coordinates is used.
		linear = 0x2601
	};

	//

	/// Container for a two-dimensional image texture accessable to shaders.
	/// @details
	/// While `tr::bitmap` is a container for two-dimensional image data in main memory, `tr::texture` is a container for two-dimensional
	/// image data stored on the graphics device. This texture data is then able to set on a shader sampler uniform location and sampled
	/// during the rendering process.
	///
	/// Strictly speaking, instances of `tr::texture` are only containers for these underlying texture objects. This means, for example,
	/// that setting a texture on a shader does not set the literal `tr::texture` object at a specific location in memory, but rather the
	/// value it contains. If the value is moved to a different instance of `tr::texture`, that value will still be set on the shader. If
	/// the instance is overriden with a new value, the old value is destroyed and the shader will no longer have a set buffer. Instances of
	/// `tr::texture` may at any point allocate new texture objects and release previously held texture objects using the `allocate()`
	/// method.
	///
	/// `tr::texture` has a notion of 'completeness'. An instance of the class may be constructed without allocating actual texture data.
	/// Such an instance is considered incomplete until the `allocate()` method is invoked. Incomplete textures may not be used for most
	/// actions.
	///
	/// Every instance of `tr::texture` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::texture` are left in a special 'invalid' state, distinct from the incomplete state. Invalid
	/// `tr::texture` instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::texture` instances may be labeled and are formattable. Example format output: `"My texture" (OpenGL ID: 5)`.
	class texture
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an incomplete texture.
		/// @param context Graphics context to create the texture on.
		/// @param label Label of the texture.
		[[nodiscard]] explicit texture(graphics_context& context, std::string_view label = {}) noexcept;

		/// Allocates an uninitialized texture.
		/// @param context Graphics context to create the texture on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture.
		[[nodiscard]] texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
							  pixel_format format = pixel_format::rgba32, std::string_view label = {});

		/// Constructs a texture with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture.
		[[nodiscard]] texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
							  std::optional<pixel_format> format = std::nullopt, std::string_view label = {});

		/// Textures are not copyable.
		texture(const texture&) = delete;

		/// Moves a texture.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Texture to move from.
		[[nodiscard]] texture(texture&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Textures are not copyable.
		texture& operator=(const texture&) = delete;

		/// Moves a texture.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Texture to move from.
		/// @return Reference to `*this`.
		texture& operator=(texture&& rhs) noexcept = default;

		/// @}
		/// @name View
		/// @{

		/// Gets a mutable view to the texture.
		/// @return Mutable view to the texture.
		[[nodiscard]] operator mutable_texture_view() noexcept;

		/// Gets a view to the texture.
		/// @return View to the texture.
		[[nodiscard]] operator texture_view() const noexcept;

		/// Gets a mutable view to the texture.
		/// @return Mutable view to the texture.
		[[nodiscard]] mutable_texture_view view() noexcept;

		/// Gets a view to the texture.
		/// @return View to the texture.
		[[nodiscard]] texture_view view() const noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the texture is on.
		/// @return Reference to the graphics context the texture is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the texture is in a valid state.
		/// @return `true` if the texture is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// Gets whether the texture is complete.
		/// @return `true` if the texture is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept;

		/// Gets the size of the texture.
		/// @return Size of the texture.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @}
		/// @name Allocation
		/// @{

		/// Allocates the texture and releases the previously held storage as a new texture.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @return Previously held storage as a new texture.
		texture allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Attributes
		/// @{

		/// Gets the pixel format of the texture.
		/// @return Pixel format of the texture.
		[[nodiscard]] pixel_format format() const noexcept;

		/// Sets the filters used by the texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		/// Sets the wrapping used by the texture sampler.
		/// @param wrap Wrapping type to use.
		void set_wrap(wrap wrap) noexcept;

		/// Sets the border color of the texture sampler (used when `wrap::border_clamp` is in use).
		/// @param color Border color to use.
		void set_border_color(rgbaf color) noexcept;

		/// @}
		/// @name Clearing & setting
		/// @{

		/// Clears the texture.
		/// @param color Color to clear the texture to.
		void clear(rgbaf color = rgba8{}) noexcept;

		/// Clears a region of the texture.
		/// @param region Region of the texture to clear.
		/// @param color Color to clear the texture region to.
		void clear_region(rectangle<int> region, rgbaf color = rgba8{}) noexcept;

		/// Copies a region from another texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param src Source texture view.
		/// @param region Region from the texture to copy.
		void copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region) noexcept;

		/// Sets a region of the texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param bitmap Bitmap data to copy to the texture.
		void set_region(glm::ivec2 tl, sub_bitmap bitmap) noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the texture.
		/// @return Debug label of the texture.
		[[nodiscard]] std::string label() const;

		/// Sets the debug label of the texture.
		/// @param label Debug label of the texture.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL texture.
		/// @note This does not release the texture.
		/// @return OpenGL texture ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Texture deleter.
		struct deleter
		{
			/// Reference to the graphics context the texture is on.
			ref<graphics_context> context;

			//

			/// Deletes a texture.
			/// @param texture OpenGL texture ID.
			void operator()(unsigned int texture) const noexcept;
		};

		//

		/// Handle to the OpenGL texture.
		mutable handle<unsigned int, 0, deleter> m_handle;

		/// Pixel format of the texture.
		pixel_format m_format;

		/// Size of the texture.
		glm::ivec2 m_size;

		//

		/// Creates a released texture.
		/// @param context Graphics context the texture is on.
		/// @param handle Handle to the OpenGL texture.
		/// @param size Cached size of the texture.
		[[nodiscard]] texture(graphics_context& context, unsigned int handle, pixel_format pixel_format, glm::ivec2 size) noexcept;

		//
	};
} // namespace tr

//

/// Texture formatter.
template <>
struct std::formatter<tr::texture>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid texture format specification."};
		}
		return context.begin();
	}

	/// Formats a texture.
	/// @tparam FormatContext Formatting context type.
	/// @param texture Texture to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::texture& texture, FormatContext& context) const
	{
		if (texture.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", texture.label(), texture.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid texture at {}>", static_cast<const void*>(&texture));
		}
	}
};