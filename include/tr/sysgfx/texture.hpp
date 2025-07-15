#pragma once
#include "../utility/handle.hpp"
#include "bitmap.hpp"

namespace tr {
	class texture_ref;
	class render_target;
	namespace imgui {
		std::uint64_t get_texture_id(texture_ref texture) noexcept;
	}
} // namespace tr

namespace tr {
	// Texture wrapping types.
	enum class wrap {
		REPEAT = 0x2901,        // The texture is repeated.
		MIRROR_REPEAT = 0x8370, // The texture is repeated and mirrored.
		EDGE_CLAMP = 0x812F,    // The value of the edge pixel is used.
		BORDER_CLAMP = 0x812D   // The value of the border color is used.
	};

	// Minifying filter types.
	enum class min_filter {
		NEAREST = 0x2600,       // The value of the texture element that is nearest to the specified texture coordinates is used.
		LINEAR = 0x2601,        // The average of the four texture elements that are closest to the specified texture coordinates is used.
		NMIP_NEAREST = 0x2700,  // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		NMIP_LINEAR = 0x2702,   // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		LMIPS_NEAREST = 0x2701, // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		LMIPS_LINEAR = 0x2703   // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
	};

	// Magnifying filter types.
	enum class mag_filter {
		NEAREST = 0x2600, // The value of the texture element that is nearest to the specified texture coordinates is used.
		LINEAR = 0x2601   // The average of the four texture elements that are closest to the specified texture coordinates is used.
	};

	// Texture swizzles.
	enum class swizzle {
		ZERO,       // The channel is set to 0.0.
		ONE,        // The channel is set to 1.0.
		R = 0x1903, // The channel will use the red channel value.
		G,          // The channel will use the green channel value.
		B,          // The channel will use the blue channel value.
		A           // The channel will use the alpha channel value.
	};

	// 2D texture living on the GPU.
	class texture {
	  public:
		// Allocates an uninitialized 2D texture.
		texture(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32) noexcept;
		// Constructs a 2D texture with data uploaded from a bitmap.
		texture(const sub_bitmap& bitmap, bool mipmapped = false, std::optional<pixel_format> format = std::nullopt) noexcept;

		// Gets the size of the texture.
		const glm::ivec2& size() const noexcept;

		// Sets the filters used by the texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;
		// Sets the wrapping used for by the texture sampler.
		void set_wrap(wrap wrap) noexcept;
		// Sets the swizzle parameters of the texture.
		void set_swizzle(swizzle r, swizzle g, swizzle b, swizzle a) noexcept;
		// Sets the border color of the texture sampler (used when wrap::BORDER_CLAMP is in use).
		void set_border_color(rgbaf color) noexcept;

		// Clears the texture.
		void clear(const rgbaf& color) noexcept;
		// Clears a region of the texture.
		void clear_region(const irect2& rect, const rgbaf& color) noexcept;
		// Copies a region from another texture.
		void copy_region(glm::ivec2 tl, const texture& src, const irect2& rect) noexcept;
		// Sets a region of the texture.
		void set_region(glm::ivec2 tl, const sub_bitmap& bitmap) noexcept;

		// Sets the debug label of the texture.
		void set_label(std::string_view label) noexcept;

	  protected:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL texture.
		handle<unsigned int, 0, deleter> _id;
		// The size of the texture.
		glm::ivec2 _size;

		// Creates an unallocated texture.
		texture() noexcept;
		// Allocates the texture.
		void _allocate(glm::ivec2 size, bool mipmapped, pixel_format format) noexcept;

		friend class dyn_atlas;
		friend class ttf_renderer;
		friend class texture_ref;
	};

	// Non-owning texture reference.
	class texture_ref {
	  public:
		// Creates an empty texture reference.
		constexpr texture_ref() noexcept = default;
		// Creates a texture reference.
		texture_ref(const texture& texture) noexcept;

		friend bool operator==(texture_ref l, texture_ref r) noexcept = default;

		// Checks whether the reference is pointing to an extant texture.
		bool valid() const noexcept;

	  private:
		// The OpenGL ID of the texture (or 0).
		unsigned int _id{0};

		friend class texture_unit;
		friend std::uint64_t imgui::get_texture_id(texture_ref texture) noexcept;
	};
	// Empty texture reference.
	constexpr texture_ref NO_TEXTURE{};

	// 2D texture that can be rendered to.
	class render_texture : public texture {
	  public:
		// Allocates an uninitialized 2D texture.
		render_texture(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32) noexcept;
		// Constructs a 2D texture with data uploaded from a bitmap.
		render_texture(const sub_bitmap& bitmap, bool mipmapped = false, std::optional<pixel_format> format = std::nullopt) noexcept;
		render_texture(render_texture&&) = default;
		~render_texture() noexcept;

		render_texture& operator=(render_texture&&) = default;

		// Gets a render target spanning the entire texture.
		operator render_target() const noexcept;
		// Gets a render target spanning the entire texture.
		render_target render_target() const noexcept;
		// Gets a render target spanning a region of the texture.
		tr::render_target region_render_target(const irect2& rect) const noexcept;

	  private:
		struct fbo_deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to an OpenGL FBO.
		handle<unsigned int, 0, fbo_deleter> _fbo;
	};
} // namespace tr