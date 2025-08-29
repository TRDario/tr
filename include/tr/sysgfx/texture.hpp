#pragma once
#include "../utility/handle.hpp"
#include "bitmap.hpp"

namespace tr {
	namespace gfx {
		class texture;
		class texture_ref;
		class render_target;
	} // namespace gfx
	namespace imgui {
		u64 get_texture_id(const gfx::texture& texture);
	}
} // namespace tr

namespace tr::gfx {
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

	// 2D texture living on the GPU.
	class texture {
	  public:
		// Creates an empty texture.
		texture();
		// Allocates an uninitialized texture.
		texture(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32);
		// Constructs a texture with data uploaded from a bitmap.
		texture(const sub_bitmap& bitmap, bool mipmapped = false, std::optional<pixel_format> format = std::nullopt);
		// Moves a texture, updating all references pointing to it.
		texture(texture&& r) noexcept;
		// Destroys the texture, emptying all references pointing to it.
		~texture();

		// Moves a texture, updating all references pointing to it.
		texture& operator=(texture&& r) noexcept;

		// Gets whether the texture is empty.
		bool empty() const;
		// Gets the size of the texture.
		const glm::ivec2& size() const;

		// Reallocates the texture and releases the previously held storage as a new texture.
		texture reallocate(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32);

		// Sets the filters used by the texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter);
		// Sets the wrapping used for by the texture sampler.
		void set_wrap(wrap wrap);
		// Sets the border color of the texture sampler (used when wrap::BORDER_CLAMP is in use).
		void set_border_color(rgbaf color);

		// Clears the texture.
		void clear(const rgbaf& color);
		// Clears a region of the texture.
		void clear_region(const irect2& rect, const rgbaf& color);
		// Copies a region from another texture.
		void copy_region(glm::ivec2 tl, const texture& src, const irect2& rect);
		// Sets a region of the texture.
		void set_region(glm::ivec2 tl, const sub_bitmap& bitmap);

		// Sets the debug label of the texture.
		void set_label(std::string_view label);

	  protected:
		// Handle to the OpenGL texture.
		unsigned int m_handle;
		// The cached size of the texture.
		glm::ivec2 m_size;
		// List of active references to this texture.
		mutable std::vector<ref<texture_ref>> m_refs;

		// Creates a released texture.
		texture(unsigned int handle, glm::ivec2 size);

		friend class texture_ref;
		friend class texture_unit;
	};

	// Smart texture reference (updated on texture moves and updates, emptied on deletion).
	class texture_ref {
	  public:
		// Creates an empty reference.
		constexpr texture_ref() = default;
		constexpr texture_ref(std::nullopt_t) {};
		// Creates a non-empty reference.
		texture_ref(const texture& tex);
		texture_ref(const texture_ref& r);
		texture_ref(texture_ref&& r) noexcept;
		~texture_ref();

		texture_ref& operator=(const texture& tex);
		texture_ref& operator=(const texture_ref& r);
		texture_ref& operator=(texture_ref&& r) noexcept;

		friend bool operator==(const texture_ref& l, const texture_ref& r) = default;

		// Checks whether the reference is empty.
		bool empty() const;

	  private:
		opt_ref<const texture> m_ref{};

		friend class texture;
		friend class texture_unit;
	};

	// 2D texture that can be rendered to.
	class render_texture : public texture {
	  public:
		// Creates an empty texture.
		render_texture() = default;
		// Allocates an uninitialized texture.
		render_texture(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32);
		// Constructs a texture with data uploaded from a bitmap.
		render_texture(const sub_bitmap& bitmap, bool mipmapped = false, std::optional<pixel_format> format = std::nullopt);
		// Moves a texture, updating all references pointing to it.
		render_texture(render_texture&&) noexcept = default;
		// Destroys the texture, emptying all references pointing to it.
		~render_texture();

		// Moves a texture, updating all references pointing to it.
		render_texture& operator=(render_texture&& r) noexcept = default;

		// Reallocates the texture and releases the previously held storage as a new texture.
		texture reallocate(glm::ivec2 size, bool mipmapped = false, pixel_format format = pixel_format::RGBA32);

		// Gets a render target spanning the entire texture.
		operator render_target() const;
		// Gets a render target spanning the entire texture.
		render_target render_target() const;
		// Gets a render target spanning a region of the texture.
		gfx::render_target region_render_target(const irect2& rect) const;

	  private:
		struct fbo_deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to an OpenGL FBO.
		handle<unsigned int, 0, fbo_deleter> m_fbo;

		using texture::reallocate;
	};
} // namespace tr::gfx