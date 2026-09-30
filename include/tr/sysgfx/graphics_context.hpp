/// @file
/// @brief Provides a window graphics context class and related datatypes.

#pragma once
#include <tr/sysgfx/blending.hpp>
#include <tr/sysgfx/dynamic_vertex_buffer.hpp>
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/sysgfx/internal/opengl.hpp>
#include <tr/sysgfx/logger.hpp>
#include <tr/sysgfx/render_target.hpp>
#include <tr/sysgfx/static_vertex_buffer.hpp>
#include <tr/sysgfx/vertex_format.hpp>
#include <tr/utility/zstring_view.hpp>

struct SDL_GLContextState;
struct SDL_Window;
namespace tr
{
	class dynamic_index_buffer;
	class shader_pipeline;
	class static_index_buffer;
	class window_view;
} // namespace tr

//

namespace tr
{
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

		/// @name Constructors and destructors
		/// @{

		/// Creates a graphics context on a window.
		/// @param window Window to create the graphics context for.
		/// @exception graphics_context_init_error If creating the graphics context failed.
		[[nodiscard]] graphics_context(window_view window);

		/// Graphics contexts are not copyable.
		graphics_context(const graphics_context&) = delete;

		/// Graphics contexts are not movable.
		graphics_context(graphics_context&&) = delete;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Graphics contexts are not copyable.
		graphics_context& operator=(const graphics_context&) = delete;

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

		/// @}
		/// @name Wireframe mode
		/// @{

		/// Gets whether wireframe rendering is enabled.
		/// @return `true` if wireframe rendering is enabled, `false` otherwise.
		[[nodiscard]] bool wireframe_mode_enabled() const noexcept;

		/// Sets whether rendering should be done as a wireframe.
		/// @param enable Whether to enable wireframe rendering.
		void set_wireframe_mode(bool enable) noexcept;

		/// @}
		/// @name Face culling
		/// @{

		/// Gets whether face culling is enabled.
		/// @return `true` if face culling is enabled, `false` otherwise.
		[[nodiscard]] bool face_culling_enabled() const noexcept;

		/// Sets whether face culling should be used.
		/// @param enable Whether to enable face culling.
		void set_face_culling(bool enable) noexcept;

		/// @}
		/// @name Depth testing
		/// @{

		/// Gets whether depth testing is enabled.
		/// @return `true` if depth testing is enabled, `false` otherwise.
		[[nodiscard]] bool depth_testing_enabled() const noexcept;

		/// Sets whether depth testing should be used.
		/// @param enable Whether to enable depth testing.
		void set_depth_testing(bool enable) noexcept;

		/// @}
		/// @name Render target
		/// @{

		/// Sets the active render target.
		/// @param target Render target to set as active.
		void set_render_target(const render_target& target) noexcept;

		/// @}
		/// @name Shader pipeline
		/// @{

		/// Sets the active shader pipeline.
		/// @param pipeline Pipeline to set as active.
		void set_shader_pipeline(const shader_pipeline& pipeline) noexcept;

		/// @}
		/// @name Blending
		/// @{

		/// Gets the active blending mode.
		/// @return Reference to the active blending mode.
		const blend_mode& blend_mode() const noexcept;

		/// Sets the active blending mode.
		/// @param blend_mode Blending mode to set as active.
		void set_blend_mode(const tr::blend_mode& blend_mode) noexcept;

		/// @}
		/// @name Vertex format
		/// @{

		/// Sets the active vertex format.
		/// @param format Vertex format to set as active.
		void set_vertex_format(const vertex_format& format) noexcept;

		/// @}
		/// @name Vertex buffers
		/// @{

		/// Sets an active vertex buffer.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to set.
		/// @param stride Stride between the elements of the vertex buffer.
		void set_vertex_buffer(const untyped_static_vertex_buffer& buffer, int slot, ssize offset, int stride) noexcept;

		/// Sets an active vertex buffer.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to set.
		/// @param stride Stride between the elements of the vertex buffer.
		void set_vertex_buffer(const untyped_dynamic_vertex_buffer& buffer, int slot, ssize offset, int stride) noexcept;

		/// Sets an active vertex buffer.
		/// @tparam Element Type of the vertex buffer elements.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to set.
		template <standard_layout Element>
		void set_vertex_buffer(const static_vertex_buffer<Element>& buffer, int slot, int offset) noexcept
		{
			TR_ASSERT(buffer.valid(), "Tried to set a vertex buffer in an invalid state to a graphics context.");
			TR_ASSERT(&buffer.context() == this, "Tried to set vertex buffer {} to a context it is not associated with.", buffer);

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			check_typed_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<Element>);
#endif
			if (m_bound_vertex_buffers.size() <= static_cast<usize>(slot)) {
				m_bound_vertex_buffers.resize(slot + 1);
			}

			bound_vertex_buffer_info& bound_vertex_buffer{m_bound_vertex_buffers[slot]};
			if (internal::graphics_object_id id{buffer.id()};
				bound_vertex_buffer.id != id || bound_vertex_buffer.offset != offset || bound_vertex_buffer.stride != sizeof(Element)) {
				TR_LOG_TRACE("gfx", "Setting vertex buffer {} to context.", buffer);
				gl().bind_vertex_buffer(slot, buffer.unwrap(), offset * sizeof(Element), sizeof(Element));
				bound_vertex_buffer.id = id;
				bound_vertex_buffer.offset = offset * sizeof(Element);
				bound_vertex_buffer.stride = sizeof(Element);
#ifdef TR_ENABLE_CHECKED_GRAPHICS
				bound_vertex_buffer.vertex_attributes = {};
#endif
			}
		}

		/// Sets an active vertex buffer.
		/// @tparam Element Type of the vertex buffer elements.
		/// @param buffer Buffer to set as active.
		/// @param slot Slot to set the buffer in.
		/// @param offset Starting offset within the buffer to set.
		template <standard_layout Element>
		void set_vertex_buffer(const dynamic_vertex_buffer<Element>& buffer, int slot, int offset) noexcept
		{
			TR_ASSERT(buffer.valid(), "Tried to set a vertex buffer in an invalid state to a graphics context.");
			TR_ASSERT(&buffer.context() == this, "Tried to set vertex buffer {} to a context it is not associated with.", buffer);

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			check_typed_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<Element>);
#endif
			if (m_bound_vertex_buffers.size() <= static_cast<usize>(slot)) {
				m_bound_vertex_buffers.resize(slot + 1);
			}

			bound_vertex_buffer_info& bound_vertex_buffer{m_bound_vertex_buffers[slot]};
			if (internal::graphics_object_id id{buffer.id()};
				bound_vertex_buffer.id != id || bound_vertex_buffer.offset != offset || bound_vertex_buffer.stride != sizeof(Element)) {
				TR_LOG_TRACE("gfx", "Setting vertex buffer {} to context.", buffer);
				gl().bind_vertex_buffer(slot, buffer.unwrap(), offset * sizeof(Element), sizeof(Element));
				bound_vertex_buffer.id = id;
				bound_vertex_buffer.offset = offset * sizeof(Element);
				bound_vertex_buffer.stride = sizeof(Element);
#ifdef TR_ENABLE_CHECKED_GRAPHICS
				bound_vertex_buffer.vertex_attributes = {};
#endif
			}
		}

		/// @name Index buffer
		/// @{

		/// Sets the active index buffer.
		/// @param buffer Buffer to set as active.
		void set_index_buffer(const static_index_buffer& buffer) noexcept;

		/// Sets the active index buffer.
		/// @param buffer Buffer to set as active.
		void set_index_buffer(const dynamic_index_buffer& buffer) noexcept;

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
		[[nodiscard]] const internal::opengl& gl() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Gets the set of framebuffers registered on this context.
		/// @return Reference to the set of framebuffers registered on this context.
		[[nodiscard]] boost::unordered_flat_set<internal::graphics_object_id>& registered_framebuffers() noexcept;

		/// Gets the set of shaders registered on this context.
		/// @return Reference to the set of shaders registered on this context.
		[[nodiscard]] boost::unordered_flat_set<internal::graphics_object_id>& registered_shaders() noexcept;

		/// Gets the set of shader pipelines registered on this context.
		/// @return Reference to the set of shader pipelines registered on this context.
		[[nodiscard]] boost::unordered_flat_set<internal::graphics_object_id>& registered_shader_pipelines() noexcept;

		/// Gets the set of vertex formats registered on this context.
		/// @return Reference to the set of vertex formats registered on this context.
		[[nodiscard]] boost::unordered_flat_set<internal::graphics_object_id>& registered_vertex_formats() noexcept;

		/// Gets the set of buffers registered on this context.
		/// @return Reference to the set of buffers registered on this context.
		[[nodiscard]] boost::unordered_flat_set<internal::graphics_object_id>& registered_buffers() noexcept;
#endif

		//

		/// Allocates a texture unit.
		/// @return Allocated texture unit index.
		unsigned int allocate_texture_unit() noexcept;

		/// Frees a texture unit.
		/// @param texture_unit Index of the texture unit.
		void free_texture_unit(unsigned int texture_unit) noexcept;

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
			/// Set of active registered framebuffer IDs.
			boost::unordered_flat_set<internal::graphics_object_id> registered_framebuffers;

			/// Set of active registered shader IDs.
			boost::unordered_flat_set<internal::graphics_object_id> registered_shaders;

			/// Set of active registered shader pipeline IDs.
			boost::unordered_flat_set<internal::graphics_object_id> registered_shader_pipelines;

			/// Set of active registered vertex format IDs.
			boost::unordered_flat_set<internal::graphics_object_id> registered_vertex_formats;

			/// Set of active registered buffer IDs.
			boost::unordered_flat_set<internal::graphics_object_id> registered_buffers;
#endif

			//

			/// Destroys a graphics context.
			/// @param context Pointer to the SDL OpenGL context.
			void operator()(SDL_GLContextState* context) const noexcept;
		};

		/// Information about a bound vertex buffer.
		struct bound_vertex_buffer_info
		{
			/// Unique graphics object ID of the vertex buffer.
			internal::graphics_object_id id{internal::graphics_object_id::invalid};

			/// Starting offset within the buffer.
			ssize offset{};

			/// Stride between the elements of the vertex buffer.
			int stride{};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Vertex attributes of the element type of the vertex buffer.
			std::span<const vertex_attribute> vertex_attributes{};
#endif
		};

		//

		/// Pointer to the window the context was created on.
		SDL_Window* m_window;

		/// Pointer to the SDL OpenGL context.
		std::unique_ptr<SDL_GLContextState, deleter> m_ptr;

		/// OpenGL function pointers.
		internal::opengl m_gl;

		/// Tracks which texture units are allocated.
		std::bitset<80> m_allocated_texture_units;

		/// Whether wireframe mode is enabled.
		bool m_wireframe_mode_enabled{false};

		/// Whether face culling is enabled.
		bool m_face_culling_enabled{false};

		/// Whether depth testing is enabled.
		bool m_depth_testing_enabled{false};

		/// Unique graphics object ID of the bound framebuffer ('invalid' is a valid value representing the backbuffer).
		internal::graphics_object_id m_bound_framebuffer{internal::graphics_object_id::invalid};

		/// Active viewport.
		rectangle<u16> m_viewport;

		/// Active scissor box.
		rectangle<u16> m_scissor_box;

		/// Unique graphics object ID of the bound shader pipeline.
		internal::graphics_object_id m_bound_shader_pipeline{internal::graphics_object_id::invalid};

		/// Active blending mode.
		tr::blend_mode m_blend_mode{blend_multiplier::one, blend_fn::add, blend_multiplier::zero,
									blend_multiplier::one, blend_fn::add, blend_multiplier::zero};

		/// Unique graphics object ID of the bound vertex format.
		internal::graphics_object_id m_bound_vertex_format{internal::graphics_object_id::invalid};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Bindings of the bound vertex format.
		std::span<const vertex_binding> m_bound_vertex_format_bindings;
#endif

		/// Information about bound vertex buffers.
		std::vector<bound_vertex_buffer_info> m_bound_vertex_buffers;

		/// Unique graphics object ID ID of the bound index buffer.
		internal::graphics_object_id m_bound_index_buffer{internal::graphics_object_id::invalid};

		//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Checks if a vertex buffer's type's attribute match those of the current vertex format.
		/// @param label Label of the vertex buffer.
		/// @param slot Slot the vertex buffer is being set to.
		/// @param vertex_attributes Vertex attribute list of the elements of the vertex buffer.
		void check_typed_vertex_buffer(const std::string& label, int slot, std::span<const vertex_attribute> vertex_attributes) noexcept;

		//

		/// Whether to check the set index buffer.
		enum class check_index_buffer : bool
		{
			no,
			yes
		};

		/// Asserts the validity of objects set to the graphics context.
		void assert_valid_drawing_state(check_index_buffer check_index_buffer) noexcept;
#endif
	};
} // namespace tr