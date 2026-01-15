///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements gpu_benchmark.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/gpu_benchmark.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

////////////////////////////////////////////////////////////// GPU BENCHMARK //////////////////////////////////////////////////////////////

tr::gfx::gpu_benchmark::gpu_benchmark()
{
	GLuint temp;
	TR_GL_CALL(glGenQueries, 1, &temp);
	m_qo.reset(temp);
}

void tr::gfx::gpu_benchmark::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteQueries, 1, &id);
}

//

void tr::gfx::gpu_benchmark::start()
{
	TR_GL_CALL(glBeginQuery, GL_TIME_ELAPSED, m_qo.get());
}

void tr::gfx::gpu_benchmark::stop()
{
	TR_GL_CALL(glEndQuery, GL_TIME_ELAPSED);
}

void tr::gfx::gpu_benchmark::fetch()
{
	i64 result;
	TR_GL_CALL(glGetQueryObjecti64v, m_qo.get(), GL_QUERY_RESULT, &result);
	if (m_durations.size() == 256) {
		m_durations.pop_front();
	}
	m_durations.emplace_back(tr::insecs{result});
}

void tr::gfx::gpu_benchmark::clear()
{
	m_durations.clear();
}

//

tr::duration tr::gfx::gpu_benchmark::latest() const
{
	return !m_durations.empty() ? m_durations.back() : duration{0};
}

tr::duration tr::gfx::gpu_benchmark::min() const
{
	return !m_durations.empty() ? *std::ranges::min_element(m_durations) : duration{0};
}

tr::duration tr::gfx::gpu_benchmark::max() const
{
	return !m_durations.empty() ? *std::ranges::max_element(m_durations) : duration{0};
}

tr::duration tr::gfx::gpu_benchmark::avg() const
{
	if (m_durations.empty()) {
		return duration{0};
	}
	else {
		return std::accumulate(m_durations.begin(), m_durations.end(), duration{0}) / m_durations.size();
	}
}

const tr::gfx::gpu_benchmark::deque& tr::gfx::gpu_benchmark::measurements() const
{
	return m_durations;
}
