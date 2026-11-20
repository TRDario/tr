/// @file
/// @brief Provides `tr::owning_shader_pipeline`.

#pragma once
#include <tr/sysgfx/shader.hpp>
#include <tr/sysgfx/shader_pipeline.hpp>

//

namespace tr
{
	/// @brief Owning container for a shader program pipeline that owns its shaders.
	/// @details
	/// A shader pipeline is an object that references vertex and fragment shader program stages that are to be used during the rendering
	/// process when the pipeline is set on the graphics context. These referenced shaders must be compatible: the outputs of the vertex
	/// shader must match the inputs of the fragment shader.
	///
	/// It's worth remembering that instances of `tr::owning_shader_pipeline` are only containers for these underlying shader pipeline
	/// objects. This means, for example, that setting a shader pipeline on a graphics context does not set the literal
	/// `tr::owning_shader_pipeline` object at a specific location in memory, but rather the value it contains. If the value is moved to a
	/// different instance of `tr::owning_shader_pipeline`, that value will still be set on the context. If the instance is overriden with a
	/// new value, the old value is destroyed and the graphics context will no longer have a set shader pipeline.
	///
	/// Every instance of `tr::owning_shader_pipeline` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::owning_shader_pipeline` are left in a special 'invalid' state, distinct from the incomplete state.
	/// Invalid `tr::owning_shader_pipeline` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()`.
	///
	/// `tr::owning_shader_pipeline` instances may be labeled and are formattable. Example format output: `"My pipeline" (ID: 3, PPO: 3)`.
	class owning_shader_pipeline
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an owning shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to move into the pipeline.
		/// @param fragment_shader Fragment shader to move into the pipeline.
		/// @param label Label of the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must both be valid and on `%context`, as well as compatible with eachother.
		[[nodiscard]] owning_shader_pipeline(graphics_context& context, vertex_shader&& vertex_shader, fragment_shader&& fragment_shader,
											 std::string_view label = {}) noexcept;

		/// @brief Owning shader pipelines are not copyable.
		owning_shader_pipeline(const owning_shader_pipeline&) = delete;

		/// @brief Owning shader pipelines are not copyable.
		owning_shader_pipeline& operator=(const owning_shader_pipeline&) = delete;

		/// @brief Constructs a owning shader pipeline by transferring ownership from another `tr::owning_shader_pipeline`.
		/// @param rhs Owning shader pipeline to move from.
		/// @par Invalidation
		/// If the shader pipeline previously held by `rhs` was set on the graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] owning_shader_pipeline(owning_shader_pipeline&& rhs) noexcept = default;

		/// @brief Destroys the currently held owning shader pipeline and transfers ownership from another `tr::owning_shader_pipeline`.
		/// @param rhs Owning shader pipeline to move from.
		/// @par Invalidation
		/// If the shader pipeline previously held by `*this` was set on a graphics context, it is unset.
		///
		/// If the shader pipeline previously held by `rhs` was set on the graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		owning_shader_pipeline& operator=(owning_shader_pipeline&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the label of the pipeline.
		/// @param label Label of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the pipeline is on.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the graphics context the pipeline is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Returns a reference to the base shader pipeline object.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the base shader pipeline object.
		[[nodiscard]] operator const shader_pipeline&() const noexcept;

		/// @brief Returns a reference to the vertex shader of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the vertex shader of the pipeline.
		[[nodiscard]] tr::vertex_shader& vertex_shader() noexcept;

		/// @brief Returns a reference to the vertex shader of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the vertex shader of the pipeline.
		[[nodiscard]] const tr::vertex_shader& vertex_shader() const noexcept;

		/// @brief Returns a reference to the fragment shader of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the fragment shader of the pipeline.
		[[nodiscard]] tr::fragment_shader& fragment_shader() noexcept;

		/// @brief Returns a reference to the fragment shader of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @return Reference to the fragment shader of the pipeline.
		[[nodiscard]] const tr::fragment_shader& fragment_shader() const noexcept;

		/// @brief Returns the label of the pipeline.
		/// @pre The shader pipeline must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the pipeline.
		[[nodiscard]] std::string label() const;

		/// @}

	  private:
		/// @brief Owned vertex shader.
		tr::vertex_shader m_vertex_shader;

		/// @brief Owned fragment shader.
		tr::fragment_shader m_fragment_shader;

		/// @brief Base shader pipeline.
		shader_pipeline m_shader_pipeline;
	};
} // namespace tr

//

/// @brief Owning shader pipeline formatter.
template <>
struct std::formatter<tr::owning_shader_pipeline> : std::formatter<tr::shader_pipeline>
{
};