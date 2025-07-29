#include "../../include/tr/audio/impl.hpp"

////////////////////////////////////////////////////////////// AUDIO COMMAND //////////////////////////////////////////////////////////////

tr::audio_command::audio_command(std::shared_ptr<base_audio_source> source, command_type type, float start, float end, duration length)
	: src{std::move(source)}, type{type}, start{.num = start}, end{.num = end}, length{length}, last_update{clock::now()}, elapsed{}
{
}

tr::audio_command::audio_command(std::shared_ptr<base_audio_source> source, command_type type, glm::vec2 start, glm::vec2 end,
								 duration length)
	: src{std::move(source)}, type{type}, start{.vec2 = start}, end{.vec2 = end}, length{length}, last_update{clock::now()}, elapsed{}
{
}

tr::audio_command::audio_command(std::shared_ptr<base_audio_source> source, command_type type, glm::vec3 start, glm::vec3 end,
								 duration length)
	: src{std::move(source)}, type{type}, start{.vec3 = start}, end{.vec3 = end}, length{length}, last_update{clock::now()}, elapsed{}
{
}

std::shared_ptr<tr::base_audio_source> tr::audio_command::source() const
{
	return src;
}

tr::audio_command::arg tr::audio_command::value()
{
	const time_point now{clock::now()};
	elapsed = std::min(elapsed + now - last_update, length);
	last_update = now;
	const float t{duration_cast<fsecs>(elapsed) / length};

	arg value;
	switch (type) {
	case command_type::POS:
	case command_type::VEL:
	case command_type::DIR:
		value.vec3 = start.vec3 + (end.vec3 - start.vec3) * t;
		break;
	case command_type::CONE_W:
		value.vec2 = start.vec2 + (end.vec2 - start.vec2) * t;
		break;
	default:
		value.num = start.num + (end.num - start.num) * t;
		break;
	}
	return value;
}

void tr::audio_command::execute()
{
	arg v{value()};
	switch (type) {
	case command_type::PITCH:
		src->set_pitch(v.num);
		break;
	case command_type::GAIN:
		src->set_gain(v.num);
		break;
	case command_type::MAX_DIST:
		src->set_max_dist(v.num);
		break;
	case command_type::ROLLOFF:
		src->set_rolloff(v.num);
		break;
	case command_type::REF_DIST:
		src->set_ref_dist(v.num);
		break;
	case command_type::OUT_CONE_GAIN:
		src->set_out_cone_gain(v.num);
		break;
	case command_type::CONE_W:
		src->set_cone_w(rads(v.vec2.x), rads(v.vec2.y));
		break;
	case command_type::POS:
		src->set_pos(v.vec3);
		break;
	case command_type::VEL:
		src->set_vel(v.vec3);
		break;
	case command_type::DIR:
		src->set_dir(v.vec3);
		break;
	}
}

bool tr::audio_command::done() const
{
	return elapsed == length;
}