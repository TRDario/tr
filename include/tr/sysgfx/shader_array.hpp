/// @file
/// @brief Provides `tr::shader_array`.

#pragma once
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_shader_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Typed shader-accessible GPU array.
	/// @details
	/// `tr::shader_array` is a special case of `tr::shader_buffer`, a graphics buffer used to store data for shader access. Shader arrays
	/// are resizable up to a maximum capacity specified during construction. Shader arrays can be set or mapped for direct access by the
	/// CPU; the map type of any given array is specified during construction and cannot be changed afterwards.
	///
	/// Strictly speaking, instances of `tr::shader_array` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a shader array on a shader does not set the literal `tr::shader_array` object at a specific location in
	/// memory, but rather the value it contains. If the value is moved to a different instance of `tr::shader_array`, that value will
	/// still be set on the shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer
	/// have a set buffer.
	///
	/// Every instance of `tr::shader_array` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::shader_array` are left in a special 'invalid' state. Invalid `tr::shader_array` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::shader_array` instances may be labeled and are formattable. Example format output: `"My array" (ID: 3, SSBO: 5)`.
	/// @tparam Element Type of the array elements.
	template <typename Element>
	class shader_array : private untyped_shader_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Allocates an uninitialized shader array.
		/// @param context Graphics context to create the array on.
		/// @param capacity Maximum capacity of the array in elements.
		/// @param map_type Type of map to create when mapping the array.
		[[nodiscard]] shader_array(graphics_context& context, usize capacity,
								   graphics_buffer_map_access map_type = graphics_buffer_map_access::write_only)
			: untyped_shader_buffer{context, 0, capacity * sizeof(Element), map_type}
		{
		}

		/// Shader arrays are not copyable.
		shader_array(const shader_array&) = delete;

		/// Moves a shader array.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Shader array to move from.
		[[nodiscard]] shader_array(shader_array&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Shader arrays are not copyable.
		shader_array& operator=(const shader_array&) = delete;

		/// Moves a shader array.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Shader array to move from.
		/// @return Reference to `*this`.
		shader_array& operator=(shader_array&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		using untyped_shader_buffer::context;

		/// @}
		/// @name State
		/// @{

		using untyped_shader_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the array.
		/// @return Size of the array in elements.
		[[nodiscard]] usize size() const noexcept
		{
			return untyped_shader_buffer::array_size() / sizeof(Element);
		}

		/// Gets the maximum capacity of the array.
		/// @return Maximum capacity of the array in elements.
		[[nodiscard]] usize capacity() const noexcept
		{
			return untyped_shader_buffer::array_capacity() / sizeof(Element);
		}

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the array.
		/// @param data Data to set the array to.
		void set(std::span<const Element> data) noexcept
		{
			untyped_shader_buffer::set_array(std::as_bytes(data));
		}

		/// Resizes the array.
		/// @param size Size of the array in elements.
		/// @pre `size` must be less than or equal to the capacity of the array.
		void resize(usize size) noexcept
		{
			untyped_shader_buffer::resize_array(size * sizeof(Element));
		}

		/// @}
		/// @name Mapping
		/// @{

		using untyped_shader_buffer::mapped;

		/// Maps the array.
		/// @return Map of the array.
		[[nodiscard]] mapped_graphics_buffer_span<Element> map()
		{
			return mapped_graphics_buffer_span<Element>{untyped_shader_buffer::map_array()};
		}

		/// @}
		/// @name Label
		/// @{

		using untyped_shader_buffer::label;

		using untyped_shader_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using untyped_shader_buffer::unwrap;

		/// @}
		/// @endcond
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_shader_buffer::id;

		/// @}
		/// @endcond
	};
} // namespace tr

//

/// Shader array formatter.
template <typename Element>
struct std::formatter<tr::shader_array<Element>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped shader array format specification."};
		}
		return context.begin();
	}

	/// Formats a shader array.
	/// @tparam FormatContext Formatting context type.
	/// @param array Shader array to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader_array<Element>& array, FormatContext& context) const
	{
		if (array.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, SSBO: {})", array.label(), std::to_underlying(array.id()),
								  array.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} shader array at {}>", tr::type_name<Element>(),
								  static_cast<const void*>(&array));
		}
	}
};