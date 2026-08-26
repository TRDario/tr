/// @file
/// @brief Provides a window graphics context class and related datatypes.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/logger.hpp"
#include "../utility/zstring_view.hpp"
#include "gl_api.hpp"
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
	class dyn_index_buffer;
	class shader_pipeline;
	class static_index_buffer;
	class window_view;
} // namespace tr
#ifdef TR_HAS_IMGUI
namespace tr::ImGui
{
	void Init(graphics_context& context);
} // namespace tr::ImGui
#endif

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
		graphics_context_init_error();

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Graphics context opening error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

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

		//

		/// Logger used by the context.
		logger logger;

		/// @name Constructors
		/// @{

		/// Creates a graphics context on a window.
		/// @param window Window to create the graphics context for.
		/// @exception graphics_context_init_error If creating the graphics context failed.
		graphics_context(window_view window);

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
		info info() const;

		/// @}
		/// @name Getters
		/// @{

		/// Gets a view to the window the context is on.
		/// @return View to the window the context is on.
		window_view window() const;

		/// Gets a render target spanning the entire backbuffer.
		/// @return Render target spanning the entire backbuffer.
		render_target backbuffer() const;

		/// Gets a commonly used 2D vertex format.
		/// @return Reference to the 2D vertex format.
		const vertex_format& vertex2_format();

		/// @}
		/// @name Renderers
		/// @{

		/// Allocates a fresh renderer ID.
		/// @return Unused renderer ID.
		renderer_id allocate_renderer_id();

		/// Checks whether the passed renderer ID is the active renderer, sets it as active and returns true if not.
		/// @param id Renderer ID to check.
		/// @return `true` if the renderer needs to be setup, `false` otherwise.
		bool should_setup_renderer(renderer_id id);

		/// @}
		/// @name Setters
		/// @{

		/// Sets whether rendering should be done as a wireframe.
		/// @param arg Whether to use wireframe rendering.
		void set_wireframe_mode(bool arg);

		/// Sets whether face culling should be used.
		/// @param arg Whether to use face culling.
		void set_face_culling(bool arg);

		/// Sets whether depth testing should be used.
		/// @param arg Whether to use depth testing.
		void set_depth_test(bool arg);

		/// Sets the active render target.
		/// @param target Render target to set as active.
		void set_render_target(const render_target& target);

		/// Sets the active shader pipeline.
		/// @param pipeline Pipeline to set as active.
		void set_shader_pipeline(const shader_pipeline& pipeline);

		/// Sets the active blending mode.
		/// @param blend_mode Blending mode to set as active.
		void set_blend_mode(const blend_mode& blend_mode);

		/// Sets the active vertex format.
		/// @param format Vertex format to set as active.
		void set_vertex_format(const vertex_format& format);

		/// Sets an active vertex buffer.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		/// @param stride Stride between the elements of the vertex buffer.
		void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride);

		/// Sets an active vertex buffer.
		/// @tparam T Vertex buffer element type.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		template <standard_layout T>
		void set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset);

		/// Sets an active vertex buffer.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		/// @param stride Stride between the elements of the vertex buffer.
		void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride);

		/// Sets an active vertex buffer.
		/// @tparam T Vertex buffer element type.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		template <standard_layout T>
		void set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset);

		/// Sets the active index buffer.
		/// @param buffer Buffer to set as active.
		void set_index_buffer(const static_index_buffer& buffer);

		/// Sets the active index buffer.
		/// @param buffer Buffer to set as active.
		void set_index_buffer(const dyn_index_buffer& buffer);

		/// @}
		/// @name Clearing
		/// @{

		/// Clears the backbuffer's color.
		/// @param color Color to clear the backbuffer to.
		void clear_backbuffer(rgbaf color = {0, 0, 0, 0});

		/// Clears the backbuffer.
		/// @param color Color to clear the backbuffer to.
		/// @param depth Depth to clear the backbuffer to.
		/// @param stencil Stencil to clear the backbuffer to.
		void clear_backbuffer(rgbaf color, double depth, int stencil);

		/// Clears a backbuffer region's color.
		/// @param region Region of the backbuffer to clear.
		/// @param color Color to clear the backbuffer region to.
		void clear_backbuffer_region(rectangle<int> region, rgbaf color = {0, 0, 0, 0});

		/// Clears a backbuffer region.
		/// @param region Region of the backbuffer to clear.
		/// @param color Color to clear the backbuffer region to.
		/// @param depth Depth to clear the backbuffer region to.
		/// @param stencil Stencil to clear the backbuffer region to.
		void clear_backbuffer_region(rectangle<int> region, rgbaf color, double depth, int stencil);

		/// @}
		/// @name Drawing
		/// @{

		/// Draws a mesh from a vertex buffer.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the vertex buffer.
		/// @param vertices Number of vertices to draw.
		void draw(primitive type, usize offset, usize vertices);

		/// Draws an instanced mesh from a vertex buffer.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the vertex buffer.
		/// @param vertices Number of vertices to draw.
		/// @param instances Number of instances to draw.
		void draw_instances(primitive type, usize offset, usize vertices, int instances);

		/// Draws an indexed mesh.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the index buffer.
		/// @param indices Number of indices to draw.
		void draw_indexed(primitive type, usize offset, usize indices);

		/// Draws an instanced indexed mesh.
		/// @param type Primitive type to draw.
		/// @param offset Starting offset within the index buffer.
		/// @param indices Number of indices to draw.
		/// @param instances Number of instances to draw.
		void draw_indexed_instances(primitive type, usize offset, usize indices, int instances);

		/// @}
		/// @cond implementation_details

		/// Sets the context as current and returns the OpenGL API.
		/// @return Refernce to the OpenGL API functions.
		const gl_api& gl() const;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Gets the graphics object registry associated with the context.
		/// @return Reference to the graphics object registry associated with the context.
		graphics_object_registry& registry();
#endif

		//

		/// Moves a label from one object to another.
		/// @param type OpenGL object type.
		/// @param old_id Old object ID.
		/// @param new_id New object ID.
		void move_label(unsigned int type, unsigned int old_id, unsigned int new_id);

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
			void operator()(SDL_GLContextState* context) const;
		};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Debug information about the set framebuffer.
		struct set_framebuffer_debug_info
		{
			/// Unique graphics object ID of the framebuffer.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the framebuffer.
			std::string label{"<backbuffer>"};
		};

		/// Debug information about the set shader pipeline.
		struct set_shader_pipeline_debug_info
		{
			/// Unique graphics object ID of the shader pipeline.
			graphics_object_id id{graphics_object_id::invalid};

			/// Label of the shader pipeline.
			std::string label{"<unbound>"};
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

		/// Commonly used 2D vertex format.
		std::optional<vertex_format> m_vertex2_format;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Debug information about the framebuffer set to the context.
		set_framebuffer_debug_info m_set_framebuffer_debug_info;

		/// Debug information about the shader pipeline set to the context.
		set_shader_pipeline_debug_info m_set_shader_pipeline_debug_info;

		/// Debug information about the vertex format set to the context.
		set_vertex_format_debug_info m_set_vertex_format_debug_info;
#endif

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Checks if a vertex buffer's type's attribute match those of the current vertex format.
		/// @param label Label of the vertex buffer.
		/// @param slot Slot the vertex buffer is being set to.
		/// @param attrs Vertex attribute list of the elements of the vertex buffer.
		void check_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs);
#endif

		/// Sets an active vertex buffer.
		/// @param buffer_id ID of the buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to bind.
		/// @param stride Stride between the elements of the vertex buffer.
		void set_vertex_buffer(unsigned int buffer_id, int slot, ssize offset, usize stride);

		//

		// Accesses `m_allocated_texture_units`.
		friend class texture_unit;

#ifdef TR_HAS_IMGUI
		// Accesses m_ptr.
		friend void ImGui::Init(graphics_context& context);
#endif
	};
} // namespace tr

#include "impl/graphics_context.hpp" // IWYU pragma: export