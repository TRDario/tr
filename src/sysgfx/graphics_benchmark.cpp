/// @file
/// @brief Implements graphics_benchmark.hpp.

#include "../../include/tr/sysgfx/graphics_benchmark.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/utility/ranges.hpp"

//

tr::graphics_benchmark::graphics_benchmark(graphics_context& context)
	: m_qo{deleter{context}}
{
	context.gl().generate_queries(1, out_handle(m_qo));
}

void tr::graphics_benchmark::deleter::operator()(unsigned int id) const
{
	context.gl().delete_queries(1, &id);
}

//

tr::graphics_context& tr::graphics_benchmark::context() const
{
	return m_qo.get_deleter().context;
}

//

void tr::graphics_benchmark::start()
{
	context().gl().begin_query(GL_TIME_ELAPSED, m_qo.get());
}

void tr::graphics_benchmark::stop()
{
	context().gl().end_query(GL_TIME_ELAPSED);
}

void tr::graphics_benchmark::fetch()
{
	i64 result;
	context().gl().get_query_object_i64v(m_qo.get(), GL_QUERY_RESULT, &result);
	if (m_durations.size() == 256) {
		m_durations.pop_front();
	}
	m_durations.emplace_back(tr::insecs{result});
}

void tr::graphics_benchmark::clear()
{
	m_durations.clear();
}

//

tr::duration tr::graphics_benchmark::latest() const
{
	return !m_durations.empty() ? m_durations.back() : duration::zero();
}

tr::duration tr::graphics_benchmark::min() const
{
	return !m_durations.empty() ? *std::ranges::min_element(m_durations) : duration::zero();
}

tr::duration tr::graphics_benchmark::max() const
{
	return !m_durations.empty() ? *std::ranges::max_element(m_durations) : duration::zero();
}

tr::duration tr::graphics_benchmark::avg() const
{
	if (m_durations.empty()) {
		return duration::zero();
	}
	else {
		return sum(m_durations, duration::zero()) / m_durations.size();
	}
}

const std::deque<tr::duration>& tr::graphics_benchmark::measurements() const
{
	return m_durations;
}
