/// @file
/// @brief Implements untyped_graphics_buffer_map.hpp.

#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/utility/exception.hpp>

//

tr::mapped_untyped_graphics_buffer_span::mapped_untyped_graphics_buffer_span(graphics_context& context, unsigned int buffer,
																			 std::span<std::byte> span) noexcept
	: m_handle{buffer, gl_buffer_object_unmapper{context}}
	, m_span{span}
{
}

void tr::mapped_untyped_graphics_buffer_span::gl_buffer_object_unmapper::operator()(unsigned int id) const noexcept
{
	context->gl().unmap_buffer(id);
}

tr::mapped_untyped_graphics_buffer_span::mapped_untyped_graphics_buffer_span(mapped_untyped_graphics_buffer_span&& rhs) noexcept
	: m_handle{std::move(rhs.m_handle)}
	, m_span{std::exchange(rhs.m_span, std::span<std::byte>{})}
{
}

tr::mapped_untyped_graphics_buffer_span& tr::mapped_untyped_graphics_buffer_span::operator=(
	mapped_untyped_graphics_buffer_span&& rhs) noexcept
{
	m_handle = std::move(rhs.m_handle);
	m_span = std::exchange(rhs.m_span, std::span<std::byte>{});
	return *this;
}

//

tr::mapped_untyped_graphics_buffer_span::iterator tr::mapped_untyped_graphics_buffer_span::begin() const noexcept
{
	return iterator{m_span.data()};
}

tr::mapped_untyped_graphics_buffer_span::const_iterator tr::mapped_untyped_graphics_buffer_span::cbegin() const noexcept
{
	return const_iterator{m_span.data()};
}

tr::mapped_untyped_graphics_buffer_span::iterator tr::mapped_untyped_graphics_buffer_span::end() const noexcept
{
	return iterator{m_span.data() + m_span.size()};
}

tr::mapped_untyped_graphics_buffer_span::const_iterator tr::mapped_untyped_graphics_buffer_span::cend() const noexcept
{
	return const_iterator{m_span.data() + m_span.size()};
}

//

tr::mapped_untyped_graphics_buffer_span::reference tr::mapped_untyped_graphics_buffer_span::front() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get front element of an empty mapped graphics buffer span.");

	return m_span.front();
}

tr::mapped_untyped_graphics_buffer_span::reference tr::mapped_untyped_graphics_buffer_span::back() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get back element of an empty mapped graphics buffer span.");

	return m_span.back();
}

tr::mapped_untyped_graphics_buffer_span::reference tr::mapped_untyped_graphics_buffer_span::at(size_type offset) const noexcept
{
	TR_ASSERT(offset < size(), "Tried to get out-of-bounds element at index {} of a mapped graphics buffer span of size {}.", offset,
			  size());

	return m_span[offset];
}

tr::mapped_untyped_graphics_buffer_span::reference tr::mapped_untyped_graphics_buffer_span::operator[](size_type offset) const noexcept
{
	return at(offset);
}

tr::mapped_untyped_graphics_buffer_span::pointer tr::mapped_untyped_graphics_buffer_span::data() const noexcept
{
	return m_span.data();
}

//

tr::mapped_untyped_graphics_buffer_span::size_type tr::mapped_untyped_graphics_buffer_span::size() const noexcept
{
	return m_span.size();
}

tr::mapped_untyped_graphics_buffer_span::size_type tr::mapped_untyped_graphics_buffer_span::size_bytes() const noexcept
{
	return m_span.size_bytes();
}

bool tr::mapped_untyped_graphics_buffer_span::empty() const noexcept
{
	return m_span.empty();
}

tr::graphics_context& tr::mapped_untyped_graphics_buffer_span::context() const noexcept
{
	TR_ASSERT(!empty(), "Tried to get the context of a mapped graphics buffer span in an invalid state.");

	return m_handle.get_deleter().context;
}

//

std::span<tr::mapped_untyped_graphics_buffer_span::element_type> tr::mapped_untyped_graphics_buffer_span::first(
	size_type count) const noexcept
{
	return m_span.first(count);
}

std::span<tr::mapped_untyped_graphics_buffer_span::element_type> tr::mapped_untyped_graphics_buffer_span::last(
	size_type count) const noexcept
{
	return m_span.last(count);
}

std::span<tr::mapped_untyped_graphics_buffer_span::element_type> tr::mapped_untyped_graphics_buffer_span::subspan(
	size_type offset, size_type count) const noexcept
{
	return m_span.subspan(offset, count);
}