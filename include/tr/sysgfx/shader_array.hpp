/// @file
/// @brief Provides `tr::shader_array`.

#pragma once
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_shader_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @brief Owning container for a typed shader-accessible GPU array.
	/// @details
	/// `tr::shader_array` is a special case of `tr::shader_buffer`, a graphics buffer used to store data for shader access. Shader arrays
	/// are resizable up to a maximum capacity specified during construction. Shader arrays can be set or mapped for direct access by the
	/// CPU; the map type of any given array is specified during construction and cannot be changed afterwards.
	///
	/// It's worth remembering that instances of `tr::shader_array` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a shader array on a shader does not set the literal `tr::shader_array` object at a specific location in
	/// memory, but rather the value it contains. If the value is moved to a different instance of `tr::shader_array`, that value will
	/// still be set on the shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer
	/// have a set buffer.
	///
	/// Every instance of `tr::shader_array` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::shader_array` are left in a special 'invalid' state. Invalid `tr::shader_array` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::shader_array` instances may be labeled and are formattable. Example format output: `"My array" (ID: 3, SSBO: 5)`.
	/// @tparam Element Type of the array elements.
	template <typename Element>
		requires std::is_trivially_copyable_v<Element> && std::is_trivially_destructible_v<Element>
	class shader_array : private untyped_shader_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an uninitialized shader array.
		/// @param context Graphics context to create the array on.
		/// @param capacity Maximum capacity of the array in elements.
		/// @param map_access Array map access type.
		/// @param label Label of the array.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] shader_array(graphics_context& context, usize capacity, access map_access = access::write_only,
								   std::string_view label = {})
			: untyped_shader_buffer{context, 0, capacity * sizeof(Element), map_access, label}
		{
		}

		/// @brief Shader arrays are not copyable.
		shader_array(const shader_array&) = delete;

		/// @brief Shader arrays are not copyable.
		shader_array& operator=(const shader_array&) = delete;

		/// @brief Constructs a shader array by transferring ownership from another `tr::shader_array`.
		/// @param rhs Shader array to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] shader_array(shader_array&& rhs) noexcept = default;

		/// @brief Destroys the currently held shader array and replaces it with one moved from another `tr::shader_array`.
		/// @param rhs Shader array to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `*this` is set on any shaders, it is unset.
		///
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		shader_array& operator=(shader_array&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the data of the array.
		/// @param data Data to set the array with.
		/// @pre
		/// The array must be in a valid state and must not be currently mapped.
		///
		/// The size of `data` must be smaller than or equal to the capacity of the array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void set(std::span<const Element> data) noexcept
		{
			untyped_shader_buffer::set_array(std::as_bytes(data));
		}

		/// @brief Resizes the array.
		/// @param size Size of the array.
		/// @pre
		/// The array must be in a valid state and must not be currently mapped.
		///
		/// `size` must be less than or equal to the capacity of the array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void resize(usize size) noexcept
		{
			untyped_shader_buffer::resize_array(size * sizeof(Element));
		}

		/// @brief Maps the array.
		/// @pre The array must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the array.
		[[nodiscard]] mapped_graphics_buffer_span<Element> map()
		{
			return mapped_graphics_buffer_span<Element>{untyped_shader_buffer::map_array()};
		}

		using untyped_shader_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using untyped_shader_buffer::operator bool;

		using untyped_shader_buffer::valid;

		using untyped_shader_buffer::context;

		/// @brief Returns the size of the array.
		/// @pre The array must be in a valid state.
		/// @return Size of the array.
		[[nodiscard]] usize size() const noexcept
		{
			return untyped_shader_buffer::array_size() / sizeof(Element);
		}

		/// @brief Returns the maximum capacity of the array.
		/// @pre The array must be in a valid state.
		/// @return Maximum capacity of the array.
		[[nodiscard]] usize capacity() const noexcept
		{
			return untyped_shader_buffer::array_capacity() / sizeof(Element);
		}

		using untyped_shader_buffer::mapped;

		using untyped_shader_buffer::label;

		/// @cond gl_interop
		using untyped_shader_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using untyped_shader_buffer::id;
		/// @endcond

		/// @}
	};
} // namespace tr

//

/// @brief Shader array formatter.
template <typename Element>
struct std::formatter<tr::shader_array<Element>>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a shader array.
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