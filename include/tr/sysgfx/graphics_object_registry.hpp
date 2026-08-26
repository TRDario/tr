/// @file
/// @brief Provides `tr::graphics_object_registry`.

#pragma once
#include "../utility/common.hpp"

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
	struct graphics_object_registry
	{
		/// Set of valid active framebuffer IDs.
		boost::unordered_flat_set<graphics_object_id> framebuffers;

		/// Set of valid active shader IDs.
		boost::unordered_flat_set<graphics_object_id> shaders;

		/// Set of valid active shader pipeline IDs.
		boost::unordered_flat_set<graphics_object_id> shader_pipelines;

		/// Set of valid active vertex format IDs.
		boost::unordered_flat_set<graphics_object_id> vertex_formats;
	};
} // namespace tr