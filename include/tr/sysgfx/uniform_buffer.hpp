/// @file
/// @brief Provides `tr::uniform_buffer`.

#pragma once
#include <tr/sysgfx/mapped_graphics_buffer_object.hpp>
#include <tr/sysgfx/untyped_uniform_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @brief Owning container for a shader uniform buffer.
	/// @details
	/// `tr::uniform_buffer` is the typed equivalent of `tr::untyped_uniform_buffer`, modeling a buffer holding a specific uniform
	/// structure.
	///
	/// It's worth remembering that instances of `tr::uniform_buffer` are only containers for these underlying buffer objects. This means,
	/// for example, that setting a uniform buffer on a shader does not set the literal `tr::uniform_buffer` object at a specific location
	/// in memory, but rather the value it contains. If the value is moved to a different instance of `tr::uniform_buffer`, that value will
	/// still be set on the shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer
	/// have a set buffer.
	///
	/// Every instance of `tr::uniform_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::uniform_buffer` are left in a special 'invalid' state. Invalid `tr::uniform_buffer` instances may not
	/// be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::uniform_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (ID: 3, UBO: 5)`.
	/// @tparam Object Object contained in the buffer.
	template <typename Object>
		requires std::is_trivially_copyable_v<Object> && std::is_trivially_destructible_v<Object>
	class uniform_buffer : private untyped_uniform_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] uniform_buffer(graphics_context& context, std::string_view label = {})
			: untyped_uniform_buffer{context, sizeof(Object), label}
		{
		}

		/// @brief Uniform buffers are not copyable.
		uniform_buffer(const uniform_buffer&) = delete;

		/// @brief Uniform buffers are not copyable.
		uniform_buffer& operator=(const uniform_buffer&) = delete;

		/// @brief Constructs a uniform buffer by transferring ownership from another `tr::uniform_buffer`.
		/// @param rhs Uniform buffer to move from.
		/// @par Invalidation
		/// If the uniform buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] uniform_buffer(uniform_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held uniform buffer and transfers ownership from another `tr::uniform_buffer`.
		/// @param rhs Uniform buffer to move from.
		/// @par Invalidation
		/// If the uniform buffer previously held by `*this` is set on any shaders, it is unset.
		///
		/// If the uniform buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		uniform_buffer& operator=(uniform_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the data of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @param data Data to copy into the buffer.
		void set(const Object& data) noexcept
		{
			untyped_uniform_buffer::set(as_bytes(data));
		}

		/// @brief Maps the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Write-only map of the buffer.
		[[nodiscard]] mapped_graphics_buffer_object<Object> map()
		{
			return mapped_untyped_graphics_buffer_span{untyped_uniform_buffer::map()};
		}

		using untyped_uniform_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using untyped_uniform_buffer::operator bool;

		using untyped_uniform_buffer::valid;

		using untyped_uniform_buffer::context;

		using untyped_uniform_buffer::mapped;

		using untyped_uniform_buffer::label;

		/// @cond gl_interop
		using untyped_uniform_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using untyped_uniform_buffer::id;
		/// @endcond

		/// @}
	};
} // namespace tr

//

/// @brief Uniform buffer formatter.
template <typename Object>
struct std::formatter<tr::uniform_buffer<Object>>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid uniform buffer format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a uniform buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Uniform buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::uniform_buffer<Object>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, UBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} uniform buffer at {}>", tr::type_name<Object>(),
								  static_cast<const void*>(&buffer));
		}
	}
};