/// @file
/// @brief Provides `tr::texture`.

#pragma once
#include <tr/sysgfx/pixel_format.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/rgba.hpp>
#include <tr/utility/unique_handle.hpp>

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
	/// @brief Texture mipmapping configuration.
	enum class mipmaps : bool
	{
		/// @brief Disabled texture minmapping.
		disabled,

		/// @brief Enabled texture minmapping.
		enabled
	};

	/// @brief Texture wrapping configuration.
	enum class wrap
	{
		/// @brief Repeated texture wrapping.
		/// @hideinitializer
		repeat = 0x2901,

		/// @brief Repeated and mirrored texture wrapping.
		/// @hideinitializer
		mirror_repeat = 0x8370,

		/// @brief Texture edge pixel wrapping.
		/// @hideinitializer
		edge_clamp = 0x812F,

		/// @brief Texture border color wrapping.
		/// @hideinitializer
		border_clamp = 0x812D
	};

	/// @brief Texture minifying filter configuration.
	enum class min_filter
	{
		/// @brief The value of the texture element that is nearest to the specified texture coordinates is used.
		/// @hideinitializer
		nearest = 0x2600,

		/// @brief The average of the four texture elements that are closest to the specified texture coordinates is used.
		/// @hideinitializer
		linear = 0x2601,

		/// @brief Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		/// @hideinitializer
		nmip_nearest = 0x2700,

		/// @brief Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		/// @hideinitializer
		nmip_linear = 0x2702,

		/// @brief Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		/// @hideinitializer
		lmips_nearest = 0x2701,

		/// @brief Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		/// @hideinitializer
		lmips_linear = 0x2703
	};

	/// @brief Texture magnifying filter configuration.
	enum class mag_filter
	{
		/// @brief The value of the texture element that is nearest to the specified texture coordinates is used.
		/// @hideinitializer
		nearest = 0x2600,

		/// @brief The average of the four texture elements that are closest to the specified texture coordinates is used.
		/// @hideinitializer
		linear = 0x2601
	};

	//

	/// @brief Owning container for a two-dimensional image texture accessable to shaders.
	/// @details
	/// While `tr::bitmap` is a container for two-dimensional image data in main memory, `tr::texture` is a container for two-dimensional
	/// image data stored on the graphics device. This texture data is then able to set on a shader sampler uniform location and sampled
	/// during the rendering process.
	///
	/// It's worth remembering that instances of `tr::texture` are only containers for these underlying texture objects. This means, for
	/// example, that setting a texture on a shader does not set the literal `tr::texture` object at a specific location in memory, but
	/// rather the value it contains. If the value is moved to a different instance of `tr::texture`, that value will still be set on the
	/// shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer have a set texture.
	/// Instances of `tr::texture` may at any point allocate new texture objects and release previously held texture objects using the
	/// `allocate()` method.
	///
	/// `tr::texture` has a notion of 'completeness'. An instance of the class may be constructed without allocating actual texture data.
	/// Such an instance is considered incomplete until the `allocate()` method is invoked. Incomplete textures may not be used for most
	/// actions.
	///
	/// Every instance of `tr::texture` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::texture` are left in a special 'invalid' state, distinct from the incomplete state. Invalid
	/// `tr::texture` instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or
	/// `operator bool()`.
	///
	/// `tr::texture` instances may be labeled and are formattable. Example format output: `"My texture" (OpenGL ID: 5)`.
	class texture
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an incomplete texture.
		/// @param context Graphics context to create the texture on.
		/// @param label Label of the texture.
		[[nodiscard]] explicit texture(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Constructs an uninitialized texture.
		/// @param context Graphics context to create the texture on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
							  pixel_format format = pixel_format::rgba32, std::string_view label = {});

		/// @brief Constructs an uninitialized texture.
		/// @param context Graphics context to create the texture on.
		/// @param size Size of the texture.
		/// @param label Label of the texture.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] texture(graphics_context& context, glm::ivec2 size, std::string_view label);

		/// @brief Constructs a texture with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
							  std::optional<pixel_format> format = std::nullopt, std::string_view label = {});

		/// @brief Constructs a texture with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param label Label of the texture.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] texture(graphics_context& context, sub_bitmap bitmap, std::string_view label);

		/// @brief Textures are not copyable.
		texture(const texture&) = delete;

		/// @brief Textures are not copyable.
		texture& operator=(const texture&) = delete;

		/// @brief Constructs a texture by transferring ownership from another `tr::texture`.
		/// @par Invalidation
		/// If the texture previously held by `rhs` was set on any framebuffers or shader samplers, it stays set.
		/// @param rhs Texture to move from.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] texture(texture&& rhs) noexcept = default;

		/// @brief Destroys the currently held texture and replaces it with one moved from another `tr::texture`.
		/// @par Invalidation
		/// If the texture previously held by `*this` was set on any framebuffers or shader samplers, it is unset. Any texture views
		/// pointing to `*this` are invalidated.
		///
		/// If the texture previously held by `rhs` was set on any framebuffers or shader samplers, it stays set.
		/// @param rhs Texture to move from.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		texture& operator=(texture&& rhs) noexcept = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a mutable view to the texture.
		/// @pre The texture must be in a valid state.
		/// @return Mutable view to the texture.
		[[nodiscard]] mutable_texture_view view() noexcept;

		/// @brief Returns a view to the texture.
		/// @pre The texture must be in a valid state.
		/// @return View to the texture.
		[[nodiscard]] texture_view view() const noexcept;

		/// @brief Returns a mutable view to the texture.
		/// @pre The texture must be in a valid state.
		/// @return Mutable view to the texture.
		[[nodiscard]] operator mutable_texture_view() noexcept;

		/// @brief Returns a view to the texture.
		/// @pre The texture must be in a valid state.
		/// @return View to the texture.
		[[nodiscard]] operator texture_view() const noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Allocates the texture and releases the previously held storage as a new texture.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @pre The texture must be in a valid state.
		/// @par Invalidation
		/// If `*this` was set on any framebuffers or shader samplers, it is unset. Any texture views pointing to `*this` are invalidated.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Previously held storage as a new texture.
		texture allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @brief Sets the filters used by the texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		/// @pre The texture must be in a valid state.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		/// @brief Sets the wrapping configuration used by the texture sampler.
		/// @param wrap Wrapping configuration to use.
		/// @pre The texture must be in a valid state.
		void set_wrap(wrap wrap) noexcept;

		/// @brief Sets the border color of the texture sampler (used when `wrap::border_clamp` is in use).
		/// @param color Border color to use.
		/// @pre The texture must be in a valid state.
		void set_border_color(rgba<float> color) noexcept;

		/// @brief Clears the texture.
		/// @param color Color to clear the texture to.
		/// @pre The texture must be in a valid state.
		void clear(rgba<float> color = {0, 0, 0, 0}) noexcept;

		/// @brief Clears a region of the texture.
		/// @param region Region of the texture to clear.
		/// @param color Color to clear the texture region to.
		/// @pre The texture must be in a valid state.
		void clear_region(rectangle<int> region, rgba<float> color = {0, 0, 0, 0}) noexcept;

		/// @brief Copies a region from another texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param src Source texture view.
		/// @param region Region from the texture to copy.
		/// @pre The texture must be in a valid state.
		void copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region) noexcept;

		/// @brief Sets a region of the texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @pre The texture must be in a valid state.
		void set_region(glm::ivec2 tl, sub_bitmap bitmap) noexcept;

		/// @brief Sets the label of the texture.
		/// @param label Label of the texture.
		/// @pre The texture must be in a valid state.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the texture is in a valid state.
		/// @return `true` if the texture is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the texture is in a valid state.
		/// @return `true` if the texture is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the texture is on.
		/// @pre The texture must be in a valid state.
		/// @return Reference to the graphics context the texture is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Checks whether the texture is complete.
		/// @pre The texture must be in a valid state.
		/// @return `true` if the texture is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept;

		/// @brief Returns the size of the texture.
		/// @pre The texture must be in a valid state.
		/// @return Size of the texture.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @brief Returns the pixel format of the texture.
		/// @pre The texture must be in a valid state.
		/// @return Pixel format of the texture.
		[[nodiscard]] pixel_format format() const noexcept;

		/// @brief Returns the label of the texture.
		/// @pre The texture must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the texture.
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL texture.
		/// @return Handle to the underlying OpenGL texture.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL texture deleter.
		struct gl_texture_deleter
		{
			/// @brief Reference to the graphics context the texture is on.
			ref<graphics_context> context;

			//

			/// @brief Destroys a texture.
			/// @param handle OpenGL texture handle.
			void operator()(unsigned int texture) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL texture.
		mutable unique_handle<unsigned int, 0, gl_texture_deleter> m_handle;

		/// @brief Pixel format of the texture.
		pixel_format m_format;

		/// @brief Size of the texture.
		glm::ivec2 m_size;

		//

		/// @brief Constructs a released texture.
		/// @param context Graphics context the texture is on.
		/// @param handle Handle to the OpenGL texture.
		/// @param size Cached size of the texture.
		[[nodiscard]] texture(graphics_context& context, unsigned int handle, pixel_format pixel_format, glm::ivec2 size) noexcept;
	};
} // namespace tr

//

/// @brief Texture formatter.
template <>
struct std::formatter<tr::texture>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a texture.
	/// @tparam FormatContext Formatting context type.
	/// @param texture Texture to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::texture& texture, FormatContext& context) const
	{
		if (texture) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", texture.label(), texture.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid texture at {}>", static_cast<const void*>(&texture));
		}
	}
};