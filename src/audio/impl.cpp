#include "../../include/tr/audio/impl.hpp"

////////////////////////////////////////////////////////////// AUDIO COMMAND //////////////////////////////////////////////////////////////

tr::_audio_command::_audio_command(std::shared_ptr<_audio_source> source, type type, float start, float end, duration length) noexcept
	: _src{std::move(source)}, _type{type}, _start{.num = start}, _end{.num = end}, _length{length}, _last_update{clock::now()}, _elapsed{}
{
}

tr::_audio_command::_audio_command(std::shared_ptr<_audio_source> source, type type, glm::vec2 start, glm::vec2 end,
								   duration length) noexcept
	: _src{std::move(source)}
	, _type{type}
	, _start{.vec2 = start}
	, _end{.vec2 = end}
	, _length{length}
	, _last_update{clock::now()}
	, _elapsed{}
{
}

tr::_audio_command::_audio_command(std::shared_ptr<_audio_source> source, type type, glm::vec3 start, glm::vec3 end,
								   duration length) noexcept
	: _src{std::move(source)}
	, _type{type}
	, _start{.vec3 = start}
	, _end{.vec3 = end}
	, _length{length}
	, _last_update{clock::now()}
	, _elapsed{}
{
}

std::shared_ptr<tr::_audio_source> tr::_audio_command::source() const noexcept
{
	return _src;
}

tr::_audio_command::_arg tr::_audio_command::_value() noexcept
{
	const time_point now{clock::now()};
	_elapsed = std::min(_elapsed + now - _last_update, _length);
	_last_update = now;
	const float t{duration_cast<fsecs>(_elapsed) / _length};

	_arg value;
	switch (_type) {
	case type::POS:
	case type::VEL:
	case type::DIR:
		value.vec3 = _start.vec3 + (_end.vec3 - _start.vec3) * t;
		break;
	case type::CONE_W:
		value.vec2 = _start.vec2 + (_end.vec2 - _start.vec2) * t;
		break;
	default:
		value.num = _start.num + (_end.num - _start.num) * t;
		break;
	}
	return value;
}

void tr::_audio_command::execute() noexcept
{
	_arg value{_value()};
	switch (_type) {
	case type::PITCH:
		_src->set_pitch(value.num);
		break;
	case type::GAIN:
		_src->set_gain(value.num);
		break;
	case type::MAX_DIST:
		_src->set_max_dist(value.num);
		break;
	case type::ROLLOFF:
		_src->set_rolloff(value.num);
		break;
	case type::REF_DIST:
		_src->set_ref_dist(value.num);
		break;
	case type::OUT_CONE_GAIN:
		_src->set_out_cone_gain(value.num);
		break;
	case type::CONE_W:
		_src->set_cone_w(rads(value.vec2.x), rads(value.vec2.y));
		break;
	case type::POS:
		_src->set_pos(value.vec3);
		break;
	case type::VEL:
		_src->set_vel(value.vec3);
		break;
	case type::DIR:
		_src->set_dir(value.vec3);
		break;
	}
}

bool tr::_audio_command::done() const noexcept
{
	return _elapsed == _length;
}