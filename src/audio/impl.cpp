#include "../../include/tr/audio/impl.hpp"

////////////////////////////////////////////////////////////// AUDIO COMMAND //////////////////////////////////////////////////////////////

tr::audio::command::command(std::shared_ptr<source_base> source, type type, float start, float end, duration length)
	: m_src{std::move(source)}
	, m_type{type}
	, m_start{.num = start}
	, m_end{.num = end}
	, m_length{length}
	, m_last_update{clock::now()}
	, m_elapsed{}
{
}

tr::audio::command::command(std::shared_ptr<source_base> source, type type, glm::vec2 start, glm::vec2 end, duration length)
	: m_src{std::move(source)}
	, m_type{type}
	, m_start{.vec2 = start}
	, m_end{.vec2 = end}
	, m_length{length}
	, m_last_update{clock::now()}
	, m_elapsed{}
{
}

tr::audio::command::command(std::shared_ptr<source_base> source, type type, glm::vec3 start, glm::vec3 end, duration length)
	: m_src{std::move(source)}
	, m_type{type}
	, m_start{.vec3 = start}
	, m_end{.vec3 = end}
	, m_length{length}
	, m_last_update{clock::now()}
	, m_elapsed{}
{
}

std::shared_ptr<tr::audio::source_base> tr::audio::command::source() const
{
	return m_src;
}

tr::audio::command::arg tr::audio::command::value()
{
	const time_point now{clock::now()};
	m_elapsed = std::min(m_elapsed + now - m_last_update, m_length);
	m_last_update = now;
	const float t{duration_cast<fsecs>(m_elapsed) / m_length};

	arg value;
	switch (m_type) {
	case type::POS:
	case type::VEL:
	case type::DIR:
		value.vec3 = m_start.vec3 + (m_end.vec3 - m_start.vec3) * t;
		break;
	case type::CONE_W:
		value.vec2 = m_start.vec2 + (m_end.vec2 - m_start.vec2) * t;
		break;
	default:
		value.num = m_start.num + (m_end.num - m_start.num) * t;
		break;
	}
	return value;
}

void tr::audio::command::execute()
{
	arg v{value()};
	switch (m_type) {
	case type::PITCH:
		m_src->set_pitch(v.num);
		break;
	case type::GAIN:
		m_src->set_gain(v.num);
		break;
	case type::MAX_DIST:
		m_src->set_max_dist(v.num);
		break;
	case type::ROLLOFF:
		m_src->set_rolloff(v.num);
		break;
	case type::REF_DIST:
		m_src->set_ref_dist(v.num);
		break;
	case type::OUT_CONE_GAIN:
		m_src->set_out_cone_gain(v.num);
		break;
	case type::CONE_W:
		m_src->set_cone_w(rads(v.vec2.x), rads(v.vec2.y));
		break;
	case type::POS:
		m_src->set_pos(v.vec3);
		break;
	case type::VEL:
		m_src->set_vel(v.vec3);
		break;
	case type::DIR:
		m_src->set_dir(v.vec3);
		break;
	}
}

bool tr::audio::command::done() const
{
	return m_elapsed == m_length;
}