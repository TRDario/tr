///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a window graphics context class and related datatypes.                                                                       //
//                                                                                                                                       //
// Graphics contexts are associated with a window and their properties depend on the graphics properties set during window construction. //
// A view to the window the graphics context belongs to can be gotten at any time afterwards:                                            //
//     - tr::graphics_context context{window} -> creates a graphics context tied to 'window'                                             //
//     - context.window() -> view to 'window'                                                                                            //
// Leaving any objects created on a graphics context alive after their context is erroneous, as is leaving a context alive after the     //
// window it was created on.                                                                                                             //
//                                                                                                                                       //
// Info about a graphics context can be gotten with .info() Info contains strings relating to the vendor, version and name of the        //
// underlying OpenGL renderer. In addition, a logger is created with each graphics context.                                              //
//                                                                                                                                       //
// References to a commonly used 2D vertex type may be gotten using .vertex2_format():                                                   //
//     - context.vertex2_format() -> binding 0 holds vec2 positions, binding 1 holds vec2 uvs, binding 2 holds rgb8 tints                //
//                                                                                                                                       //
// To allow for renderers to avoid having to set up graphical context on every draw call, graphics contexts provide an 'active renderer' //
// flag, which renderers should check for and set with .should_setup_renderer(id), and only set the context up if that returns true.     //
// All built-in renderers expect you to set this flag before meddling with the context, so don't forget to do that.                      //
// Allocating renderer IDs is done with .allocate_renderer_id():                                                                         //
//     - tr::renderer_id my_renderer_id{context.allocate_renderer_id()} -> allocates a new renderer ID                                   //
//     - if (context.should_setup_renderer(my_renderer_id)) { /* DO STUFF */ } -> graphical context setup                                //
//                                                                                                                                       //
// A few features of the rendering pipeline can be enabled or disabled:                                                                  //
//     - context.set_wireframe_mode(true) -> enables wireframe mode, onyl the edges of triangles are drawn                               //
//     - context.set_face_culling(true) -> enables culling of faces facing away from the camera                                          //
//     - context.set_depth_test(true) -> enables depth testing                                                                           //
//                                                                                                                                       //
// A number of components of the rendering pipeline can be set:                                                                          //
//     - context.set_render_target(target) -> sets the target to draw to                                                                 //
//     - context.set_shader_pipeline(pipeline) -> sets the shader pipeline that will be used during drawing                              //
//     - context.set_tessellation_patch_size(4) -> sets the number of vertices in a tessellation patch                                   //
//     - context.set_blend_mode(mode) -> sets the blending mode                                                                          //
//     - context.set_vertex_format(format) -> sets the expected format of vertex data                                                    //
//     - context.set_vertex_buffer(buffer, 0, 100) -> sets a buffer vertex data is pulled from, starting at offset 100, in slot 0        //
//     - context.set_index_buffer(buffer) -> sets the buffer index data is pulled from                                                   //
//                                                                                                                                       //
// After setting up the graphical context, one of the four drawing functions may be called:                                              //
//     - context.draw(tr::primitive::tri_fan, 0, 4)                                                                                      //
//       -> draws a triangle fan from the set vertex buffer                                                                              //
//     - context.draw_indexed(tr::primitive::tris, 10, 15)                                                                               //
//       -> draws 5 triangles using data from the set vertex and index buffers, starting from index 10 in the index buffer               //
//     - context.draw_instances(tr::primitive::line_loop, 0, 10, 10)                                                                     //
//       -> draws 10 instances of a line loop from the set vertex buffer                                                                 //
//     - context.draw_indexed_instances(tr::primitive::line_strip, 0, 10, 10)                                                            //
//       -> draws 10 instances of a line strip using data from the set vertex and index buffers                                          //
//                                                                                                                                       //
// Each context holds a backbuffer, and a render target corresponding to it can be gotten with .backbuffer().                            //
// The only direct way of manipulating the backbuffer's contents is by clearing it or a region of it.                                    //
// This can be done for just the color component, or all 3 of the backbuffer components:                                                 //
//     - context.backbuffer() -> gets the backbuffer render target                                                                       //
//     - context.clear_backbuffer() -> clears the backbuffer to transparency                                                             //
//     - context.clear_backbuffer({255, 255, 255, 255}) -> clears the backbuffer to white                                                //
//     - context.clear_backbuffer({255, 255, 255, 255}, 1.0f, 0) -> clears the backbuffer to white, depth 1.0f, and stencil 0            //
//     - context.clear_backbuffer_region({{100, 100}, {100, 100}})                                                                       //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to transparency                                                   //
//     - context.clear_backbuffer_region({{100, 100}, {100, 100}}, {255, 0, 0, 255})                                                     //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to red                                                            //
//     - context.clear_backbuffer_region({{100, 100}, {100, 100}}, {255, 0, 0, 255}, 1.0f, 0)                                            //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to red, depth 1.0f, and stencil 0                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/exception.hpp"
#include "../utility/logger.hpp"
#include "../utility/zstring_view.hpp"
#include "gl_api.hpp"
#include "render_target.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

struct SDL_GLContextState;
struct SDL_Window;
namespace tr {
	struct blend_mode;
	class dyn_index_buffer;
	class shader_pipeline;
	class static_index_buffer;
	class window_view;
} // namespace tr
#ifdef TR_HAS_IMGUI
namespace tr::ImGui {
	void Init(graphics_context& context);
	void Draw(graphics_context& context);
} // namespace tr::ImGui
#endif

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Renderer ID.
	enum class renderer_id : u32 {
		no_renderer,   // No particular renderer is being used.
		imgui_renderer // tr::ImGui::Draw
	};

	// Rendering primitives.
	enum class primitive {
		points,      // The vertices are drawn as individual points.
		lines,       // The vertices are drawn in pairs as lines.
		line_loop,   // The vertices are drawn as a continuous line loop.
		line_strip,  // The vertices are drawn as a continuous line strip.
		tris,        // The vertices are drawn in groups of three as triangles.
		tri_strip,   // The vertices are drawn as a continuous triangle strip.
		tri_fan,     // The vertices are drawn as a continuous triangle fan.
		patches = 14 // The vertices are sent to the tessellation shaders as patches.
	};

	// Graphics context initialization error.
	class graphics_context_init_error : public exception {
	  public:
		// Constructs a graphics context initialization error.
		graphics_context_init_error();

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Description of the error.
		std::string m_description;
	};

	// Window graphics context.
	class graphics_context {
	  public:
		// Info returned by info().
		struct info {
			// Context vendor name.
			zstring_view vendor;
			// Context renderer name.
			zstring_view renderer;
			// Context OpenGL version.
			zstring_view gl_version;
		};

		// Creates a graphics context on a window.
		// May throw: graphics_context_init_error.
		graphics_context(window_view window);
		// Graphics contexts are not movable.
		graphics_context(graphics_context&&) = delete;

		// Graphics contexts are not movable.
		graphics_context& operator=(graphics_context&&) = delete;

		// Logger used by the context.
		logger logger;

		// Gets info about the context.
		info info() const;

		// Gets a view to the window the context is on.
		window_view window() const;
		// Gets a render target spanning the entire backbuffer.
		render_target backbuffer() const;
		// Gets a commonly used 2D vertex format.
		const vertex_format& vertex2_format();

		// Allocates a fresh renderer ID.
		renderer_id allocate_renderer_id();
		// Checks whether the passed renderer ID is the active renderer, sets it as active and returns true if not.
		bool should_setup_renderer(renderer_id id);

		// Sets whether rendering should be done as a wireframe.
		void set_wireframe_mode(bool arg);
		// Sets whether face culling should be used.
		void set_face_culling(bool arg);
		// Sets whether depth testing should be used.
		void set_depth_test(bool arg);

		// Sets the active render target.
		void set_render_target(const render_target& target);
		// Sets the active shader pipeline.
		void set_shader_pipeline(const shader_pipeline& pipeline);
		// Sets the active blending mode.
		void set_blend_mode(const blend_mode& blend_mode);
		// Sets the active vertex format.
		void set_vertex_format(const vertex_format& format);
		// Sets an active vertex buffer.
		void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride);
		// Sets an active vertex buffer.
		template <standard_layout T> void set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset);
		// Sets an active vertex buffer.
		void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride);
		// // Sets an active vertex buffer.
		template <standard_layout T> void set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset);
		// Sets the active index buffer.
		void set_index_buffer(const static_index_buffer& buffer);
		// Sets the active index buffer.
		void set_index_buffer(const dyn_index_buffer& buffer);

		// Clears the backbuffer's color.
		void clear_backbuffer(const rgbaf& color = {0, 0, 0, 0});
		// Clears the backbuffer.
		void clear_backbuffer(const rgbaf& color, double depth, int stencil);
		// Clears a backbuffer region's color.
		void clear_backbuffer_region(const rectangle<int>& region, const rgbaf& color = {0, 0, 0, 0});
		// Clears a backbuffer region.
		void clear_backbuffer_region(const rectangle<int>& region, const rgbaf& color, double depth, int stencil);

		// Draws a mesh from a vertex buffer.
		void draw(primitive type, usize offset, usize vertices);
		// Draws an instanced mesh from a vertex buffer.
		void draw_instances(primitive type, usize offset, usize vertices, int instances);
		// Draws an indexed mesh.
		void draw_indexed(primitive type, usize offset, usize indices);
		// Draws an instanced indexed mesh.
		void draw_indexed_instances(primitive type, usize offset, usize indices, int instances);

	  private:
		// Context deleter.
		struct deleter {
			void operator()(SDL_GLContextState* context) const;
		};

		// Pointer to the window the context was created on.
		SDL_Window* m_window;
		// Pointer to the SDL OpenGL context.
		std::unique_ptr<SDL_GLContextState, deleter> m_ptr;
		// OpenGL function pointers.
		gl_api m_gl_api;
		// Next available renderer id.
		renderer_id m_next_renderer_id{2};
		// ID of the current active renderer.
		renderer_id m_active_renderer{renderer_id::no_renderer};
		// The current render target.
		std::optional<render_target> m_render_target;
		// Tracks which texture units are allocated.
		std::bitset<80> m_allocated_texture_units{};
		// Commonly used 2D vertex format.
		std::optional<vertex_format> m_vertex2_format;
#ifdef TR_ENABLE_GL_CHECKS
		// Bindings of the last bound vertex format.
		std::span<const vertex_binding> m_vertex_format_bindings;
		// Label of the last bound vertex format.
		std::string m_vertex_format_label;
#endif

		// Sets the context as current and returns the OpenGL API.
		const gl_api& make_current_and_return_gl_api() const;

		// Checks the render target's FBO ID.
		bool is_fbo_of_render_target(unsigned int fbo);
		// Clears the render target.
		void clear_render_target();

#ifdef TR_ENABLE_GL_CHECKS
		// Checks if a vertex buffer's type's attribute match those of the current vertex format.
		void check_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs);
#endif

		// Moves a label from one object to another.
		void move_label(unsigned int type, unsigned int old_id, unsigned int new_id);

		friend class basic_dyn_vertex_buffer;
		friend class basic_graphics_buffer_map;
		friend class basic_shader_buffer;
		friend class basic_static_vertex_buffer;
		friend class basic_uniform_buffer;
		friend class dyn_index_buffer;
		friend class framebuffer;
		friend class graphics_benchmark;
		friend class graphics_buffer;
		friend class shader_base;
		friend class shader_pipeline;
		friend class static_index_buffer;
		friend class texture;
		friend class texture_unit;
		friend class vertex_format;
#ifdef TR_HAS_IMGUI
		friend void ImGui::Init(graphics_context& context);
		friend void ImGui::Draw(graphics_context& context);
#endif
	};
} // namespace tr

#include "impl/graphics_context.hpp" // IWYU pragma: export