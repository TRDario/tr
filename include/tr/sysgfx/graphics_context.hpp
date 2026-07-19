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
#include "render_target.hpp"
#include "texture_ref.hpp"
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
		const tr::vertex_format& vertex2_format();

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

		// Structure holding OpenGL function pointers.
		struct glapi {
			using debug_callback = void (*)(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length,
											const char* message, const void* userParam);

			void (*allocate_2d_texture_storage)(unsigned int texture, int levels, unsigned int internalformat, int width, int height);
			void (*allocate_buffer_storage)(unsigned int buffer, std::intptr_t size, const void* data, unsigned int flags);
			void (*begin_query)(unsigned int target, unsigned int id);
			void (*bind_buffer)(unsigned int target, unsigned int buffer);
			void (*bind_buffer_base)(unsigned int target, unsigned int index, unsigned int buffer);
			void (*bind_buffer_range)(unsigned int target, unsigned int index, unsigned int buffer, std::intptr_t offset,
									  std::intptr_t size);
			void (*bind_framebuffer)(unsigned int target, unsigned int framebuffer);
			void (*bind_program_pipeline)(unsigned int pipeline);
			void (*bind_textures)(unsigned int first, int count, const unsigned int* textures);
			void (*bind_vertex_array)(unsigned int array);
			void (*bind_vertex_buffer)(unsigned int bindingindex, unsigned int buffer, std::intptr_t offset, int stride);
			void (*clear)(unsigned int mask);
			void (*clear_texture_image)(unsigned int texture, int level, unsigned int format, unsigned int type, const void* data);
			void (*clear_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int zoffset, int width, int height,
											int depth, unsigned int format, unsigned int type, const void* data);
			void (*copy_image_sub_data)(unsigned int srcName, unsigned int srcTarget, int srcLevel, int srcX, int srcY, int srcZ,
										unsigned int dstName, unsigned int dstTarget, int dstLevel, int dstX, int dstY, int dstZ,
										int srcWidth, int srcHeight, int srcDepth);
			void (*create_buffers)(int n, unsigned int* buffers);
			void (*create_framebuffers)(int n, unsigned int* framebuffers);
			void (*create_program_pipelines)(int n, unsigned int* pipelines);
			unsigned int (*create_shader_program_v)(unsigned int type, int count, const char** strings);
			void (*create_textures)(unsigned int target, int n, unsigned int* textures);
			void (*create_vertex_arrays)(int n, unsigned int* arrays);
			void (*delete_buffers)(int n, const unsigned int* buffers);
			void (*delete_framebuffers)(int n, const unsigned int* framebuffers);
			void (*delete_program)(unsigned int program);
			void (*delete_program_pipelines)(int n, const unsigned int* pipelines);
			void (*delete_queries)(int n, const unsigned int* queries);
			void (*delete_textures)(int n, const unsigned int* textures);
			void (*delete_vertex_arrays)(int n, const unsigned int* arrays);
			void (*disable)(unsigned int cap);
			void (*draw_arrays)(unsigned int mode, int first, int count);
			void (*draw_arrays_instanced)(unsigned int mode, int first, int count, int instancecount);
			void (*draw_elements)(unsigned int mode, int count, unsigned int type, const void* indices);
			void (*draw_elements_instanced)(unsigned int mode, int count, unsigned int type, const void* indices, int instancecount);
			void (*enable)(unsigned int cap);
			void (*enable_vertex_array_attribute)(unsigned int vaobj, unsigned int index);
			void (*end_query)(unsigned int target);
			void (*generate_queries)(int n, unsigned int* ids);
			void (*generate_texture_mipmap)(unsigned int texture);
			unsigned int (*get_error)();
			void (*get_buffer_parameter_iv)(unsigned int buffer, unsigned int pname, int* params);
			void (*get_integer_v)(unsigned int pname, int* data);
			void (*get_object_label)(unsigned int identifier, unsigned int name, int bufSize, int* length, char* label);
			void (*get_program_info_log)(unsigned int program, int maxLength, int* length, char* infoLog);
			void (*get_program_interface_iv)(unsigned int program, unsigned int programInterface, unsigned int pname, int* params);
			void (*get_program_iv)(unsigned int program, unsigned int pname, int* params);
			void (*get_program_resource_iv)(unsigned int program, unsigned int programInterface, unsigned int index, int propCount,
											const unsigned int* props, int bufSize, int* length, int* params);
			void (*get_program_resource_name)(unsigned int program, unsigned int programInterface, unsigned int index, int bufSize,
											  int* length, char* name);
			void (*get_query_object_i64v)(unsigned int id, unsigned int pname, std::int64_t* params);
			const unsigned char* (*get_string)(unsigned int name);
			void (*get_texture_parameter_fv)(unsigned int texture, unsigned int pname, float* params);
			void (*get_texture_parameter_iv)(unsigned int texture, unsigned int pname, int* params);
			void (*invalidate_buffer_data)(unsigned int buffer);
			void* (*map_buffer_range)(unsigned int buffer, std::intptr_t offset, std::intptr_t length, unsigned int access);
			void (*set_2d_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int width, int height,
											 unsigned int format, unsigned int type, const void* pixels);
			void (*set_buffer_sub_data)(unsigned int buffer, std::intptr_t offset, std::intptr_t size, const void* data);
			void (*set_clear_color)(float red, float green, float blue, float alpha);
			void (*set_clear_depth)(double depth);
			void (*set_clear_stencil)(int s);
			void (*set_debug_message_callback)(debug_callback callback, const void* userParam);
			void (*set_debug_message_control)(unsigned int source, unsigned int type, unsigned int severity, int count,
											  const unsigned int* ids, bool enabled);
			void (*set_framebuffer_texture)(unsigned int framebuffer, unsigned int attachment, unsigned int texture, int level);
			void (*set_object_label)(unsigned int identifier, unsigned int name, int length, const char* label);
			void (*set_pixel_store_i)(unsigned int pname, int param);
			void (*set_polygon_mode)(unsigned int face, unsigned int mode);
			void (*set_program_uniform_1f)(unsigned int program, int location, float v0);
			void (*set_program_uniform_1fv)(unsigned int program, int location, int count, const float* value);
			void (*set_program_uniform_2f)(unsigned int program, int location, float v0, float v1);
			void (*set_program_uniform_2fv)(unsigned int program, int location, int count, const float* value);
			void (*set_program_uniform_3f)(unsigned int program, int location, float v0, float v1, float v2);
			void (*set_program_uniform_3fv)(unsigned int program, int location, int count, const float* value);
			void (*set_program_uniform_4f)(unsigned int program, int location, float v0, float v1, float v2, float v3);
			void (*set_program_uniform_4fv)(unsigned int program, int location, int count, const float* value);
			void (*set_program_uniform_1i)(unsigned int program, int location, int v0);
			void (*set_program_uniform_1iv)(unsigned int program, int location, int count, const int* value);
			void (*set_program_uniform_2i)(unsigned int program, int location, int v0, int v1);
			void (*set_program_uniform_2iv)(unsigned int program, int location, int count, const int* value);
			void (*set_program_uniform_3i)(unsigned int program, int location, int v0, int v1, int v2);
			void (*set_program_uniform_3iv)(unsigned int program, int location, int count, const int* value);
			void (*set_program_uniform_4i)(unsigned int program, int location, int v0, int v1, int v2, int v3);
			void (*set_program_uniform_4iv)(unsigned int program, int location, int count, const int* value);
			void (*set_program_uniform_1ui)(unsigned int program, int location, unsigned int v0);
			void (*set_program_uniform_1uiv)(unsigned int program, int location, int count, const unsigned int* value);
			void (*set_program_uniform_2ui)(unsigned int program, int location, unsigned int v0, unsigned int v1);
			void (*set_program_uniform_2uiv)(unsigned int program, int location, int count, const unsigned int* value);
			void (*set_program_uniform_3ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2);
			void (*set_program_uniform_3uiv)(unsigned int program, int location, int count, const unsigned int* value);
			void (*set_program_uniform_4ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2,
											unsigned int v3);
			void (*set_program_uniform_4uiv)(unsigned int program, int location, int count, const unsigned int* value);
			void (*set_program_uniform_matrix2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix2x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix2x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix3x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix3x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix4x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_program_uniform_matrix4x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
			void (*set_scissor)(int x, int y, int width, int height);
			void (*set_separate_blend_equations)(unsigned int modeRGB, unsigned int modeAlpha);
			void (*set_separate_blend_function)(unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha);
			void (*set_texture_parameter_fv)(unsigned int texture, unsigned int pname, const float* params);
			void (*set_texture_parameter_i)(unsigned int texture, unsigned int pname, int param);
			void (*set_vertex_array_attribute_binding)(unsigned int vaobj, unsigned int attribindex, unsigned int bindingindex);
			void (*set_vertex_array_attribute_format)(unsigned int vaobj, unsigned int attribindex, int size, unsigned int type,
													  bool normalized, unsigned int relativeoffset);
			void (*set_vertex_array_binding_divisor)(unsigned int vaobj, unsigned int bindingindex, unsigned int divisor);
			void (*set_viewport)(int x, int y, int width, int height);
			bool (*unmap_buffer)(unsigned int buffer);
			void (*use_program_stages)(unsigned int pipeline, unsigned int stages, unsigned int program);

			// Loads OpenGL function pointers.
			glapi();
		};

		// Pointer to the window the context was created on.
		SDL_Window* m_window;
		// Pointer to the SDL OpenGL context.
		std::unique_ptr<SDL_GLContextState, deleter> m_ptr;
		// OpenGL function pointers.
		glapi m_glapi;
		// Next available renderer id.
		renderer_id m_next_renderer_id{2};
		// ID of the current active renderer.
		renderer_id m_active_renderer{renderer_id::no_renderer};
		// The current render target.
		std::optional<render_target> m_render_target;
		// Tracks which texture units are allocated and what the textures bound to them are.
		std::array<std::optional<texture_ref>, 80> m_texture_units{};
		// Commonly used 2D vertex format.
		std::optional<tr::vertex_format> m_vertex2_format;
#ifdef TR_ENABLE_GL_CHECKS
		// Bindings of the last bound vertex format.
		std::span<const vertex_binding> m_vertex_format_bindings;
		// Label of the last bound vertex format.
		std::string m_vertex_format_label;
#endif

		// Sets the context as current and returns the OpenGL API.
		const glapi& make_current_and_return_glapi() const;

		// Checks the render target's FBO ID.
		bool is_fbo_of_render_target(unsigned int fbo);
		// Clears the render target.
		void clear_render_target();

		// Allocates a texture unit.
		unsigned int allocate_texture_unit();
		// Sets the texture bound to a texture unit.
		void set_texture_unit(unsigned int unit, texture_ref texture);
		// Frees a texture unit.
		void free_texture_unit(unsigned int id);
		// Rebinds texture units bound to a texture that got reallocated.
		void rebind_texture_units(const texture& texture);

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
		friend class graphics_benchmark;
		friend class graphics_buffer;
		friend class render_texture;
		friend class shader_base;
		friend class shader_pipeline;
		friend class static_index_buffer;
		friend class texture;
		friend class vertex_format;
#ifdef TR_HAS_IMGUI
		friend void ImGui::Init(graphics_context& context);
		friend void ImGui::Draw(graphics_context& context);
#endif
	};
} // namespace tr