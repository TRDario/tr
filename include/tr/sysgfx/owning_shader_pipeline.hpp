/// @file
/// @brief Provides `tr::owning_shader_pipeline`.

#pragma once
#include <tr/sysgfx/shader.hpp>
#include <tr/sysgfx/shader_pipeline.hpp>

//

namespace tr
{
	/// Container for a shader program pipeline that owns its shaders.
	/// @details
	/// Most things brought up in the description of `tr::shader_pipeline` apply to this class as well, though instances of
	/// `tr::owning_shader_pipeline` are always complete.
	class owning_shader_pipeline
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an owning shader pipeline.
		/// @param context Graphics context to create the pipeline on.
		/// @param vertex_shader Vertex shader to move into the pipeline.
		/// @param fragment_shader Fragment shader to move into the pipeline.
		/// @pre `%vertex_shader` and `%fragment_shader` must be valid shaders and be on `%context`.
		[[nodiscard]] owning_shader_pipeline(graphics_context& context, vertex_shader&& vertex_shader,
											 fragment_shader&& fragment_shader) noexcept;

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets the base pipeline object.
		/// @return Reference to the base pipeline object.
		[[nodiscard]] operator const shader_pipeline&() const noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the pipeline is on.
		/// @return Reference to the graphics context the pipeline is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Shaders
		/// @{

		/// Gets the vertex shader.
		/// @return Reference to the vertex shader of the pipeline.
		[[nodiscard]] vertex_shader& vertex_shader() noexcept;

		/// Gets the vertex shader.
		/// @return Reference to the vertex shader of the pipeline.
		[[nodiscard]] const tr::vertex_shader& vertex_shader() const noexcept;

		/// Gets the fragment shader.
		/// @return Reference to the fragment shader of the pipeline.
		[[nodiscard]] fragment_shader& fragment_shader() noexcept;

		/// Gets the fragment shader.
		/// @return Reference to the fragment shader of the pipeline.
		[[nodiscard]] const tr::fragment_shader& fragment_shader() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the pipeline is in a valid state.
		/// @return `true` if the pipeline is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the pipeline.
		/// @param label Debug label of the pipeline.
		void set_label(std::string_view label) noexcept;

		/// Gets the debug label of the pipeline.
		/// @return Debug label of the pipeline.
		[[nodiscard]] std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL shader pipeline.
		/// @note This does not release the shader pipeline.
		/// @return OpenGL shader pipeline ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Held vertex shader.
		tr::vertex_shader m_vertex_shader;

		/// Held fragment shader.
		tr::fragment_shader m_fragment_shader;

		/// Base shader pipeline.
		shader_pipeline m_shader_pipeline;
	};
} // namespace tr

//

/// Owning shader pipeline formatter.
template <>
struct std::formatter<tr::owning_shader_pipeline>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid owning shader pipeline format specification."};
		}
		return context.begin();
	}

	/// Formats a shader pipeline.
	/// @tparam FormatContext Formatting context type.
	/// @param pipeline Shader pipeline to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::owning_shader_pipeline& pipeline, FormatContext& context) const
	{
		if (pipeline.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", pipeline.label(), pipeline.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid owning shader pipeline at {}>", static_cast<const void*>(&pipeline));
		}
	}
};