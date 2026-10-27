/// @file
/// @brief Implements timer_query.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/timer_query.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::timer_query::timer_query(graphics_context& context) noexcept
	: m_handle{deleter{context}}
{
	context.gl().generate_queries(1, out_handle(m_handle));
}

void tr::timer_query::deleter::operator()(unsigned int qo) const noexcept
{
	context->gl().delete_queries(1, &qo);
}

//

void tr::timer_query::begin() noexcept
{
	TR_ASSERT(valid(), "Tried to begin a query on a timer query in an invalid state.");

	context().gl().begin_query(GL_TIME_ELAPSED, unwrap());
}

void tr::timer_query::end() noexcept
{
	TR_ASSERT(valid(), "Tried to end a query on a timer query in an invalid state.");

	context().gl().end_query(unwrap());
}

//

bool tr::timer_query::has_value() const noexcept
{
	TR_ASSERT(valid(), "Tried to check for value on a timer query in an invalid state.");

	i64 result;
	context().gl().get_query_object_i64v(unwrap(), GL_QUERY_RESULT_AVAILABLE, &result);
	return static_cast<bool>(result);
}

tr::nsecs<tr::i64> tr::timer_query::value() const noexcept
{
	TR_ASSERT(valid(), "Tried to get value on a timer query in an invalid state.");

	i64 result{};
	context().gl().get_query_object_i64v(unwrap(), GL_QUERY_RESULT, &result);
	return nsecs<tr::i64>{result};
}

//

std::string tr::timer_query::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a timer query in an invalid state.");

	const internal::opengl& gl{context().gl()};
	int label_length;
	gl.get_object_label(GL_QUERY, m_handle.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_QUERY, m_handle.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::timer_query::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of a timer query in an invalid state.");

	context().gl().set_object_label(GL_QUERY, unwrap(), static_cast<int>(label.size()), label.data());
}

//

unsigned int tr::timer_query::unwrap() const noexcept
{
	return m_handle.get();
}