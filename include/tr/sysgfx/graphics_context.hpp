///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functionality related to the graphical context.                                                                              //
//                                                                                                                                       //
// To allow for renderers to avoid having to set up graphical context on every draw call, tr provides a global 'active renderer' flag,   //
// which renderers should check for and set with tr::gfx::should_setup_context, and only set the context up if that returns true.        //
// All built-in renderers expect you to set this flag before meddling with the context, so don't forget to do that.                      //
// Allocating renderer IDs is done with tr::gfx::allocate_renderer_id:                                                                   //
//     - tr::gfx::renderer_id my_renderer_id{tr::gfx::allocate_renderer_id()} -> allocates a new renderer ID                             //
//     - if (tr::gfx::should_setup_context(my_renderer_id)) { /* DO STUFF */ } -> graphical context setup                                //
//                                                                                                                                       //
// A few features of the rendering pipeline can be enabled or disabled:                                                                  //
//     - tr::gfx::set_wireframe_mode(true) -> enables wireframe mode, onyl the edges of triangles are drawn                              //
//     - tr::gfx::set_face_culling(true) -> enables culling of faces facing away from the camera                                         //
//     - tr::gfx::set_depth_test(true) -> enables depth testing                                                                          //
//                                                                                                                                       //
// A number of components of the rendering pipeline can be set:                                                                          //
//     - tr::gfx::set_render_target(target) -> sets the target to draw to                                                                //
//     - tr::gfx::set_shader_pipeline(pipeline) -> sets the shader pipeline that will be used during drawing                             //
//     - tr::gfx::set_tessellation_patch_size(4) -> sets the number of vertices in a tessellation patch                                  //
//     - tr::gfx::set_blend_mode(mode) -> sets the blending mode                                                                         //
//     - tr::gfx::set_vertex_format(format) -> sets the expected format of vertex data                                                   //
//     - tr::gfx::set_vertex_buffer(buffer, 0, 100) -> sets a buffer vertex data is pulled from, starting at offset 100, in slot 0       //
//     - tr::gfx::set_index_buffer(buffer) -> sets the buffer index data is pulled from                                                  //
//                                                                                                                                       //
// After setting up the graphical context, one of the four drawing functions may be called:                                              //
//     - tr::gfx::draw(tr::gfx::primitive::tri_fan, 0, 4)                                                                                //
//       -> draws a triangle fan from the set vertex buffer                                                                              //
//     - tr::gfx::draw_indexed(tr::gfx::primitive::tris, 10, 15)                                                                         //
//       -> draws 5 triangles using data from the set vertex and index buffers, starting from index 10 in the index buffer               //
//     - tr::gfx::draw_instances(tr::gfx::primitive::line_loop, 0, 10, 10)                                                               //
//       -> draws 10 instances of a line loop from the set vertex buffer                                                                 //
//     - tr::gfx::draw_indexed_instances(tr::gfx::primitive::line_strip, 0, 10, 10)                                                      //
//       -> draws 10 instances of a line strip using data from the set vertex and index buffers                                          //
//                                                                                                                                       //
// If TR_ENABLE_ASSERTS is defined, tr may output information relating to the graphical subsystem to tr::gfx::log, which may freely be   //
// redirected to file or disabled, much like tr::log.                                                                                    //
//     - tr::gfx::log = tr::logger{"gl", "logs/gl.txt"} -> the tr::gfx logger will now output to logs/gl.txt                             //
//                                                                                                                                       //
// The TR_SET_LABEL macro is provided as a way to set the label of graphical resources when TR_ENABLE_ASSERTS is defined.                //
// If TR_ENABLE_ASSERTS is not defined, the macro does nothing.                                                                          //
//     - TR_SET_LABEL(my_shader, "My Shader") -> calls my_shader.set_label("My Shader") with TR_ENABLE_ASSERTS, does nothing otherwise   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

namespace tr::gfx {
	struct blend_mode;
	class dyn_index_buffer;
	class render_target;
	class shader_pipeline;
	class static_index_buffer;
} // namespace tr::gfx

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Renderer ID.
	enum class renderer_id : u32 {
		no_renderer,   // No particular renderer is being used.
		imgui_renderer // tr::ImGui::Draw
	};
	// Allocates a fresh renderer ID.
	renderer_id allocate_renderer_id();
	// Checks whether the passed renderer ID is the active renderer, sets it as active and returns true if not.
	bool should_setup_context(renderer_id id);

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
	// Sets the number of vertices per tessellation patch.
	void set_tessellation_patch_size(int vertices);
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

	// Draws a mesh from a vertex buffer.
	void draw(primitive type, usize offset, usize vertices);
	// Draws an instanced mesh from a vertex buffer.
	void draw_instances(primitive type, usize offset, usize vertices, int instances);
	// Draws an indexed mesh.
	void draw_indexed(primitive type, usize offset, usize indices);
	// Draws an instanced indexed mesh.
	void draw_indexed_instances(primitive type, usize offset, usize indices, int instances);

#ifdef TR_ENABLE_ASSERTS
	// The graphics context log.
	inline logger log{"gl"};

// Sets an object's label.
#define TR_SET_LABEL(object, label) (object).set_label(label)
#else
#define TR_SET_LABEL(object, label)                                                                                                        \
	do {                                                                                                                                   \
	} while (0)
#endif
} // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_GL_CHECKS
namespace tr::gfx {
	// Sets an active vertex buffer.
	void set_vertex_buffer_checked(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride,
								   std::span<const vertex_attribute> attributes);
	// Sets an active vertex buffer.
	void set_vertex_buffer_checked(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride,
								   std::span<const vertex_attribute> attributes);
} // namespace tr::gfx
#endif

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	set_vertex_buffer_checked(buffer, slot, offset * sizeof(T), sizeof(T), as_vertex_attribute_list<T>);
#else
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
#endif
}

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	set_vertex_buffer_checked(buffer, slot, offset * sizeof(T), sizeof(T), as_vertex_attribute_list<T>);
#else
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
#endif
}