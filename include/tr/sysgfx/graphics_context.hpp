/// @file
/// @brief Provides a window graphics context class and related datatypes.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"
#include "gl_api.hpp"
#include "index_buffer.hpp"
#include "render_target.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"
#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include "graphics_object_registry.hpp"
#endif

struct SDL_GLContextState;
struct SDL_Window;
namespace tr
{
	struct blend_mode;
	class shader_pipeline;
	class window_view;
} // namespace tr

//

namespace tr
{
	/// Renderer ID.
	enum class renderer_id : u32
	{
		/// No particular renderer is being used.
		no_renderer,

		/// `tr::ImGui::Draw`.
		imgui_renderer
	};

	/// Rendering primitives.
	enum class primitive
	{
		/// The vertices are drawn as individual points.
		points,

		/// The vertices are drawn in pairs as lines.
		lines,

		/// The vertices are drawn as a continuous line loop.
		line_loop,

		/// The vertices are drawn as a continuous line strip.
		line_strip,

		/// The vertices are drawn in groups of three as triangles.
		tris,

		/// The vertices are drawn as a continuous triangle strip.
		tri_strip,

		/// The vertices are drawn as a continuous triangle fan.
		tri_fan,

		/// The vertices are sent to the tessellation shaders as patches.
		patches = 14
	};

	//

	/// Graphics context initialization error.
	class graphics_context_init_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a graphics context initialization error.
		[[nodiscard]] graphics_context_init_error();

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Graphics context opening error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;
	};

	//

	/// Window graphics context.
	/// @details
	/// Graphics contexts are associated with a window and their properties depend on the graphics properties set during window
	/// construction.
	class graphics_context
	{
	  public:
		/// Info returned by `info()`.
		struct info
		{
			/// Context vendor name.
			zstring_view vendor;

			/// Context renderer name.
			zstring_view renderer;

			/// Context OpenGL version.
			zstring_view gl_version;
		};

		/// @name Constructors
		/// @{

		/// Creates a graphics context on a window.
		/// @param window Window to create the graphics context for.
		/// @exception graphics_context_init_error If creating the graphics context failed.
		[[nodiscard]] graphics_context(window_view window);

		/// Graphics contexts are not movable.
		graphics_context(graphics_context&&) = delete;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Graphics contexts are not movable.
		graphics_context& operator=(graphics_context&&) = delete;

		/// @}
		/// @name Information
		/// @{

		/// Gets info about the context.
		/// @return Info about the context.
		[[nodiscard]] info info() const noexcept;

		/// @}
		/// @name Getters
		/// @{

		/// Gets a view to the window the context is on.
		/// @return View to the window the context is on.
		[[nodiscard]] window_view window() const noexcept;

		/// Gets a render target spanning the entire backbuffer.
		/// @return Render target spanning the entire backbuffer.
		[[nodiscard]] render_target backbuffer() const noexcept;

		/// Gets a reference to a vertex format describing an array of vec2.
		/// @return Reference to the vec2 vertex format.
		[[nodiscard]] const vertex_format& vec2_vertex_format() noexcept;

		/// Gets a reference to a vertex format descibing arrays of vec2 (position), vec2 (uv) and rgba8 (color).
		/// @return Reference to the 2D vertex format.
		[[nodiscard]] const vertex_format& basic_2d_vertex_format() noexcept;

		/// @}
		/// @name Renderers
		/// @{

		/// Allocates a fresh renderer ID.
		/// @return Unused renderer ID.
		[[nodiscard]] renderer_id allocate_renderer_id() noexcept;

		/// Checks whether the passed renderer ID is the active renderer, sets it as active and returns true if not.
		/// @param id Renderer ID to check.
		/// @return `true` if the renderer needs to be setup, `false` otherwise.
		[[nodiscard]] bool should_setup_renderer(renderer_id id) noexcept;

		/// @}
		/// @name Setters
		/// @{

		/// Sets whether rendering should be done as a wireframe.
		/// @param arg Whether to use wireframe rendering.
		void set_wireframe_mode(bool arg) noexcept;

		/// Sets whether face culling should be used.
		/// @param arg Whether to use face culling.
		void set_face_culling(bool arg) noexcept;

		/// Sets whether depth testing should be used.
		/// @param arg Whether to use depth testing.
		void set_depth_test(bool arg) noexcept;

		/// Sets the active render target.
		/// @param target Render target to set as active.
		void set_render_target(const render_target& target) noexcept;

		/// Sets the active shader pipeline.
		/// @param pipeline Pipeline to set as active.
		void set_shader_pipeline(const shader_pipeline& pipeline) noexcept;

		/// Sets the active blending mode.
		/// @param blend_mode Blending mode to set as active.
		void set_blend_mode(const blend_mode& blend_mode) noexcept;

		/// Sets the active vertex format.
		/// @param format Vertex format to set as active.
		void set_vertex_format(const vertex_format& format) noexcept;

		/// Sets an active vertex buffer.
		/// @tparam UntypedVertexBuffer Untyped vertex buffer type.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		/// @param stride Stride between the elements of the vertex buffer.
		template <any_untyped_vertex_buffer UntypedVertexBuffer>
		void set_vertex_buffer(const UntypedVertexBuffer& buffer, int slot, ssize offset, usize stride) noexcept;

		/// Sets an active vertex buffer.
		/// @tparam TypedVertexBuffer Typed vertex buffer type.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		template <any_typed_vertex_buffer TypedVertexBuffer>
		void set_vertex_buffer(const TypedVertexBuffer& buffer, int slot, ssize offset) noexcept;

		/// Sets the active index buffer.
		/// @tparam IndexBuffer Index buffer type.
		/// @param buffer Buffer to set as active.
		template <any_index_buffer IndexBuffer>
		void set_index_buffer(const IndexBuffer& buffer) noexcept;

		/// @}
		/// @name Clearing
		/// @{

		/// Clears the backbuffer's color.
		/// @param color Color to clear the backbuffer to.
		void clear_backbuffer(rgbaf color = {0, 0, 0, 0}) noexcept;

		/// Clears the backbuffer.
		/// @param color Color to clear the backbuffer to.
		/// @param depth Depth to clear the backbuffer to.
		/// @param stencil Stencil to clear the backbuffer to.
		void clear_backbuffer(rgbaf color, double depth, int stencil) noexcept;

		/// Clears a backbuffer region's color.
		/// @param region Region of the backbuffer to clear.
		/// @param color Color to clear the backbuffer region to.
		void clear_backbuffer_region(rectangle<int> region, rgbaf color = {0, 0, 0, 0}) noexcept;

		/// Clears a backbuffer region.
		/// @param region Region of the backbuffer to clear.
		/// @param color Color to clear the backbuffer region to.
		/// @param depth Depth to clear the backbuffer region to.
		/// @param stencil Stencil to clear the backbuffer region to.
		void clear_backbuffer_region(rectangle<int> region, rgbaf color, double depth, int stencil) noexcept;

		/// @}
		/// @name Drawing
		/// @{

		/// Draws a mesh from a vertex buffer.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the vertex buffer.
		/// @param vertices Number of vertices to draw.
		void draw(primitive type, usize offset, usize vertices) noexcept;

		/// Draws an instanced mesh from a vertex buffer.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the vertex buffer.
		/// @param vertices Number of vertices to draw.
		/// @param instances Number of instances to draw.
		void draw_instances(primitive type, usize offset, usize vertices, int instances) noexcept;

		/// Draws an indexed mesh.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the index buffer.
		/// @param indices Number of indices to draw.
		void draw_indexed(primitive type, usize offset, usize indices) noexcept;

		/// Draws an instanced indexed mesh.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the index buffer.
		/// @param indices Number of indices to draw.
		/// @param instances Number of instances to draw.
		void draw_indexed_instances(primitive type, usize offset, usize indices, int instances) noexcept;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL OpenGL context pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the SDL OpenGL context.
		[[nodiscard]] SDL_GLContextState* unwrap() const noexcept;

		/// @}
		/// @endcond
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Sets the context as current and returns the OpenGL API.
		/// @return Refernce to the OpenGL API functions.
		[[nodiscard]] const gl_api& gl() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Gets the graphics object registry associated with the context.
		/// @return Reference to the graphics object registry associated with the context.
		[[nodiscard]] graphics_object_registry& registry() noexcept;
#endif

		//

		/// Moves a label from one object to another.
		/// @param type OpenGL object type.
		/// @param old_id Old object ID.
		/// @param new_id New object ID.
		void move_label(unsigned int type, unsigned int old_id, unsigned int new_id);

		/// @}
		/// @endcond

	  private:
		/// Context deleter.
		struct deleter
		{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Registry of objects created on the context.
			graphics_object_registry registry;
#endif

			//

			/// Destroys a graphics context.
			/// @param context Pointer to the SDL OpenGL context.
			void operator()(SDL_GLContextState* context) const noexcept;
		};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Debug information about an object set on the context.
		struct set_object_debug_info
		{
			/// Unique graphics object ID of the object.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the object.
			std::string label{"<unset>"};
		};

		/// Debug information about the set vertex format.
		struct set_vertex_format_debug_info
		{
			/// Unique graphics object ID of the vertex format.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the vertex format.
			std::string label{"<unbound>"};

			/// Bindings of the vertex format.
			std::span<const vertex_binding> bindings;
		};
#endif

		//

		/// Pointer to the window the context was created on.
		SDL_Window* m_window;

		/// Pointer to the SDL OpenGL context.
		std::unique_ptr<SDL_GLContextState, deleter> m_ptr;

		/// OpenGL function pointers.
		gl_api m_gl_api;

		/// Next available renderer id.
		renderer_id m_next_renderer_id{2};

		/// ID of the current active renderer.
		renderer_id m_active_renderer{renderer_id::no_renderer};

		/// Tracks which texture units are allocated.
		std::bitset<80> m_allocated_texture_units;

		/// Vertex format description an array of vec2.
		std::optional<vertex_format> m_vec2_vertex_format;

		/// Vertex format descibing arrays of vec2 (position), vec2 (uv) and rgba8 (color).
		std::optional<vertex_format> m_basic_2d_vertex_format;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Debug information about the framebuffer set to the context.
		set_object_debug_info m_set_framebuffer_debug_info{.label{"<backbuffer>"}};

		/// Debug information about the shader pipeline set to the context.
		set_object_debug_info m_set_shader_pipeline_debug_info;

		/// Debug information about the vertex format set to the context.
		set_vertex_format_debug_info m_set_vertex_format_debug_info;

		/// Debug information about the vertex buffer set to the context.
		set_object_debug_info m_set_vertex_buffer_debug_info;

		/// Debug information about the index buffer set to the context.
		set_object_debug_info m_set_index_buffer_debug_info;
#endif

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Checks if a vertex buffer's type's attribute match those of the current vertex format.
		/// @param label Label of the vertex buffer.
		/// @param slot Slot the vertex buffer is being set to.
		/// @param attrs Vertex attribute list of the elements of the vertex buffer.
		void check_typed_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs) noexcept;
#endif

		/// Sets an active vertex buffer.
		/// @tparam VertexBuffer Vertex buffer type.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer in bytes to bind.
		/// @param stride Stride between the elements of the vertex buffer.
		template <any_vertex_buffer VertexBuffer>
		void set_vertex_buffer_base(const VertexBuffer& buffer, int slot, ssize offset, usize stride) noexcept;

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Whether to check the set index buffer.
		enum class check_index_buffer : bool
		{
			no,
			yes
		};

		/// Asserts the validity of objects set to the graphics context.
		void assert_valid_drawing_state(check_index_buffer check_index_buffer) noexcept;
#endif

		//

		// Accesses `m_allocated_texture_units`.
		friend class texture_unit;
	};
} // namespace tr

#include "impl/graphics_context.hpp" // IWYU pragma: export