/// @file
/// @brief Provides `tr::graphics_object_registry`.

#pragma once
#include "../utility/integer.hpp"

//

namespace tr
{
	/// Opaque ID for graphics objects.
	enum class graphics_object_id : unsigned int
	{
		/// Sentinel value for an invalid graphics object ID.
		invalid
	};

	/// Owning handle to a unique graphics object ID.
	class graphics_object_id_handle
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a new graphics object ID handle.
		graphics_object_id_handle();

		/// Moves a graphics object ID handle.
		/// @param rhs Handle to move.
		graphics_object_id_handle(graphics_object_id_handle&& rhs);

		/// @}
		/// @name Assignment operators
		/// @{

		/// Moves a graphics object ID handle.
		/// @param rhs Handle to move.
		graphics_object_id_handle& operator=(graphics_object_id_handle&& rhs);

		/// @}
		/// @name Constructors
		/// @{

		/// Gets the base graphics object ID.
		/// @return Base ID.
		operator graphics_object_id() const;

		/// @}

	  private:
		/// Base ID.
		graphics_object_id m_id;
	};

	//

	/// Graphics object registry used to track the validity of graphics objects when `TR_ENABLE_GL_CHECKS` is enabled.
	class graphics_object_registry
	{
	  public:
		/// Gets the number of registered shaders.
		/// @return Number of registered shaders.
		usize registered_shader_count() const;

		/// Checks if a shader ID is valid.
		/// @param id Graphics object ID of the shader.
		/// @return `true` if the shader ID is valid, `false` otherwise.
		bool is_shader_valid(graphics_object_id id) const;

		/// Registers a shader.
		/// @param id Graphics object ID of the shader.
		/// @param glid OpenGL shader program ID.
		void register_shader(graphics_object_id id, unsigned int glid);

		/// Unregisters a shader.
		/// @param id Graphics object ID of the shader.
		void unregister_shader(graphics_object_id id);

		//

		/// Gets the number of registered shader pipelines.
		/// @return Number of registered shader pipelines.
		usize registered_shader_pipeline_count() const;

		/// Checks if a shader pipeline ID is valid.
		/// @param id Graphics object ID of the shader pipeline.
		/// @return `true` if the shader pipeline ID is valid, `false` otherwise.
		bool is_shader_pipeline_valid(graphics_object_id id) const;

		/// Registers a shader pipeline.
		/// @param id Graphics object ID of the shader pipeline.
		/// @param glid OpenGL shader pipeline ID.
		void register_shader_pipeline(graphics_object_id id, unsigned int glid);

		/// Unregisters a shader pipeline.
		/// @param id Graphics object ID of the shader pipeline.
		void unregister_shader_pipeline(graphics_object_id id);

		//

		/// Gets the number of registered vertex formats.
		/// @return Number of registered vertex formats.
		usize registered_vertex_format_count() const;

		/// Checks if a vertex format ID is valid.
		/// @param id Graphics object ID of the vertex format.
		/// @return `true` if the vertex format ID is valid, `false` otherwise.
		bool is_vertex_format_valid(graphics_object_id id) const;

		/// Registers a vertex format.
		/// @param id Graphics object ID of the vertex format.
		/// @param glid OpenGL vertex format ID.
		void register_vertex_format(graphics_object_id id, unsigned int glid);

		/// Unregisters a vertex format.
		/// @param id Graphics object ID of the vertex format.
		void unregister_vertex_format(graphics_object_id id);

	  private:
		/// Shader id -> glid map.
		boost::unordered_flat_map<graphics_object_id, unsigned int> m_shaders;

		/// Shader pipeline id -> glid map.
		boost::unordered_flat_map<graphics_object_id, unsigned int> m_shader_pipelines;

		/// Vertex format id -> glid map.
		boost::unordered_flat_map<graphics_object_id, unsigned int> m_vertex_formats;
	};
} // namespace tr