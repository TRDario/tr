///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements source.hpp and owning_source from audio/impl.hpp.                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/source.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include <AL/alext.h>

////////////////////////////////////////////////////////////// OWNING SOURCE //////////////////////////////////////////////////////////////

tr::audio::owning_source::owning_source(int priority)
	: m_gain{1.0f}, m_priority{priority}, m_mutex_refc{0}
{
	TR_AL_CALL(alGenSources, 1, &m_id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio source allocation"};
	}
}

tr::audio::owning_source::~owning_source()
{
	TR_AL_CALL(alDeleteSources, 1, &m_id);
}

//

void tr::audio::owning_source::use(const audio::buffer& buffer)
{
	clear();
	TR_AL_CALL(alSourcei, m_id, AL_BUFFER, ALuint(buffer.m_id.get()));
	ALint channels;
	TR_AL_CALL(alGetBufferi, ALuint(buffer.m_id.get()), AL_CHANNELS, &channels);
	TR_AL_CALL(alSourcei, m_id, AL_DIRECT_CHANNELS_SOFT, channels == 2);
}

void tr::audio::owning_source::use(std::unique_ptr<stream>&& stream)
{
	lock_audio_mutex();
	clear();
	m_stream.emplace(std::move(stream));
	unlock_audio_mutex();
}

void tr::audio::owning_source::clear()
{
	lock_audio_mutex();
	stop();
	if (m_stream.has_value()) {
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
		m_stream.reset();
	}
	else if (buffer() != buffer::id::empty) {
		set_loop_points(start, end);
		TR_AL_CALL(alSourcei, m_id, AL_LOOPING, 0);
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
	}
	unlock_audio_mutex();
}

//

int tr::audio::owning_source::priority() const
{
	return m_priority;
}

const std::bitset<32>& tr::audio::owning_source::classes() const
{
	return m_classes;
}

void tr::audio::owning_source::set_classes(const std::bitset<32>& classes)
{
	m_classes = classes;
	set_gain(gain());
}

//

float tr::audio::owning_source::pitch() const
{
	float pitch;
	TR_AL_CALL(alGetSourcef, m_id, AL_PITCH, &pitch);
	return pitch;
}

void tr::audio::owning_source::set_pitch(float pitch)
{
	TR_ASSERT(pitch > 0.0f, "Tried to set audio source pitch to invalid value '{}'.", pitch);

	TR_AL_CALL(alSourcef, m_id, AL_PITCH, pitch);
}

//

float tr::audio::owning_source::gain() const
{
	return m_gain;
}

void tr::audio::owning_source::set_gain(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set audio source gain to invalid value '{}'.", gain);

	m_gain = gain;
	TR_AL_CALL(alSourcef, m_id, AL_GAIN, g_manager.gain_multiplier(m_classes) * gain);
}

//

float tr::audio::owning_source::max_distance() const
{
	float distance;
	TR_AL_CALL(alGetSourcef, m_id, AL_MAX_DISTANCE, &distance);
	return distance;
}

void tr::audio::owning_source::set_max_distance(float distance)
{
	TR_ASSERT(distance >= 0.0f, "Tried to set audio source max distance to invalid value '{}'.", distance);

	TR_AL_CALL(alSourcef, m_id, AL_MAX_DISTANCE, distance);
}

//

float tr::audio::owning_source::rolloff_factor() const
{
	float rolloff;
	TR_AL_CALL(alGetSourcef, m_id, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}

void tr::audio::owning_source::set_rolloff_factor(float rolloff)
{
	TR_ASSERT(rolloff >= 0.0f, "Tried to set audio source rolloff factor to invalid value '{}'.", rolloff);

	TR_AL_CALL(alSourcef, m_id, AL_ROLLOFF_FACTOR, rolloff);
}

//

float tr::audio::owning_source::reference_distance() const
{
	float ref_dist;
	TR_AL_CALL(alGetSourcef, m_id, AL_REFERENCE_DISTANCE, &ref_dist);
	return ref_dist;
}

void tr::audio::owning_source::set_reference_distance(float distance)
{
	TR_ASSERT(distance >= 0.0f, "Tried to set audio source reference distance to invalid value '{}'.", distance);

	TR_AL_CALL(alSourcef, m_id, AL_REFERENCE_DISTANCE, distance);
}

//

float tr::audio::owning_source::gain_outside_cone() const
{
	float out_gain;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_OUTER_GAIN, &out_gain);
	return out_gain;
}

void tr::audio::owning_source::set_gain_outside_cone(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set audio source gain outside cone to invalid value '{}'.", gain);

	TR_AL_CALL(alSourcef, m_id, AL_CONE_OUTER_GAIN, gain);
}

//

tr::angle tr::audio::owning_source::inner_cone_width() const
{
	float width;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_INNER_ANGLE, &width);
	return degs(width);
}

tr::angle tr::audio::owning_source::outer_cone_width() const
{
	float width;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_OUTER_ANGLE, &width);
	return degs(width);
}

void tr::audio::owning_source::set_cone_widths(angle inner, angle outer)
{
	TR_ASSERT(inner >= 0_deg && inner < 360_deg, "Tried to set audio source inner cone width to invalid value '{:d}'.", inner);
	TR_ASSERT(outer >= 0_deg && outer < 360_deg, "Tried to set audio source outer cone width to invalid value '{:d}'.", outer);
	TR_ASSERT(inner < outer, "Tried to set audio source outer cone as thinner than inner cone (inner: {:d}, outer: {:d}).", inner, outer);

	TR_AL_CALL(alSourcef, m_id, AL_CONE_INNER_ANGLE, inner.degs());
	TR_AL_CALL(alSourcef, m_id, AL_CONE_OUTER_ANGLE, outer.degs());
}

//

glm::vec3 tr::audio::owning_source::position() const
{
	glm::vec3 pos;
	TR_AL_CALL(alGetSourcefv, m_id, AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio::owning_source::set_position(const glm::vec3& pos)
{
	TR_AL_CALL(alSourcefv, m_id, AL_POSITION, value_ptr(pos));
}

//

glm::vec3 tr::audio::owning_source::velocity() const
{
	glm::vec3 vel;
	TR_AL_CALL(alGetSourcefv, m_id, AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio::owning_source::set_velocity(const glm::vec3& vel)
{
	TR_AL_CALL(alSourcefv, m_id, AL_VELOCITY, value_ptr(vel));
}

//

glm::vec3 tr::audio::owning_source::direction() const
{
	glm::vec3 dir;
	TR_AL_CALL(alGetSourcefv, m_id, AL_DIRECTION, value_ptr(dir));
	return dir;
}

void tr::audio::owning_source::set_direction(const glm::vec3& dir)
{
	TR_AL_CALL(alSourcefv, m_id, AL_DIRECTION, value_ptr(dir));
}

//

tr::audio::origin tr::audio::owning_source::origin() const
{
	ALint origin;
	TR_AL_CALL(alGetSourcei, m_id, AL_SOURCE_RELATIVE, &origin);
	return audio::origin(origin);
}

void tr::audio::owning_source::set_origin(audio::origin type)
{
	TR_AL_CALL(alSourcei, m_id, AL_SOURCE_RELATIVE, ALint(type));
}

//

tr::audio::state tr::audio::owning_source::state() const
{
	ALint state;
	TR_AL_CALL(alGetSourcei, m_id, AL_SOURCE_STATE, &state);
	switch (state) {
	case AL_INITIAL:
		return state::initial;
	case AL_PLAYING:
		return state::playing;
	case AL_PAUSED:
		return state::paused;
	case AL_STOPPED:
		return state::stopped;
	default:
		return state::initial;
	}
}

void tr::audio::owning_source::play()
{
	lock_audio_mutex();
	if (m_stream.has_value()) {
		if (state() == state::initial || state() == state::stopped) {
			TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
			for (auto& buffer : m_stream->buffers) {
				buffer.refill_from(*m_stream->stream);
				TR_AL_CALL(alSourceQueueBuffers, m_id, 1, (const ALuint*)&buffer);
				if (m_stream->stream->tell() == m_stream->stream->length()) {
					break;
				}
			}
		}
	}
	TR_AL_CALL(alSourcePlay, m_id);
	unlock_audio_mutex();
}

void tr::audio::owning_source::pause()
{
	TR_AL_CALL(alSourcePause, m_id);
}

void tr::audio::owning_source::stop()
{
	if (m_stream.has_value()) {
		lock_audio_mutex();
		TR_AL_CALL(alSourceStop, m_id);
		m_stream->stream->seek(m_stream->stream->loop_start());
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourceStop, m_id);
	}
}

//

tr::fsecs tr::audio::owning_source::length() const
{
	if (m_stream.has_value()) {
		return fsecs{float(m_stream->stream->length()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != buffer::id::empty) {
		ALint sample_rate, size;
		TR_AL_CALL(alGetBufferi, m_id, AL_FREQUENCY, &sample_rate);
		TR_AL_CALL(alGetBufferi, m_id, AL_SIZE, &size);
		return sample_rate == 0 ? fsecs::zero() : fsecs{double(size) / sample_rate};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio::owning_source::offset() const
{
	float offset;
	TR_AL_CALL(alGetSourcef, m_id, AL_SEC_OFFSET, &offset);

	if (m_stream.has_value()) {
		const audio::state state{this->state()};
		if (state == state::initial || state == state::stopped) {
			return fsecs{m_stream->stream->tell() / float(m_stream->stream->sample_rate())};
		}

		auto& buffer{*std::ranges::find(m_stream->buffers, this->buffer())};
		return fsecs{buffer.start_offset / float(m_stream->stream->sample_rate()) + offset};
	}
	else {
		return fsecs{offset};
	}
}

void tr::audio::owning_source::set_offset(fsecs offset)
{
	if (m_stream.has_value()) {
		lock_audio_mutex();
		audio::state state{this->state()};
		m_stream->stream->seek(int(offset.count() * m_stream->stream->sample_rate()));
		TR_AL_CALL(alSourceStop, m_id);
		switch (state) {
		case state::playing:
			play();
			break;
		case state::paused:
			play();
			pause();
			break;
		case state::initial:
		case state::stopped:
			break;
		}
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcef, m_id, AL_SEC_OFFSET, offset.count());
	}

	TR_AL_CALL(alSourcef, m_id, AL_SEC_OFFSET, offset.count());
}

//

bool tr::audio::owning_source::looping() const
{
	if (m_stream.has_value()) {
		return m_stream->stream->looping();
	}
	else {
		ALint looping;
		TR_AL_CALL(alGetSourcei, m_id, AL_LOOPING, &looping);
		return looping;
	}
}

tr::fsecs tr::audio::owning_source::loop_start() const
{
	if (m_stream.has_value()) {
		return fsecs{float(m_stream->stream->loop_start()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != buffer::id::empty) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, ALuint(buffer()), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, ALuint(buffer()), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, m_id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{float(loop_points[0]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio::owning_source::loop_end() const
{
	if (m_stream.has_value()) {
		return fsecs{float(m_stream->stream->loop_end()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != buffer::id::empty) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, ALuint(buffer()), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, ALuint(buffer()), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, m_id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{float(loop_points[1]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

void tr::audio::owning_source::set_loop_points(fsecs start_point, fsecs end_point)
{
	if (length() == fsecs::zero()) {
		return;
	}

	start_point = std::clamp(start_point, start_point, length());
	end_point = std::clamp(end_point, start_point, length());
	TR_ASSERT(start_point < end_point, "Tried to set audio source loop end before start (start: {}s, end: {}s).", start_point.count(),
			  end_point.count());

	if (m_stream.has_value()) {
		stream& stream{*m_stream->stream};
		lock_audio_mutex();
		if (start_point >= loop_end()) {
			stream.set_loop_end(int(end_point.count() * stream.sample_rate()));
			stream.set_loop_start(int(start_point.count() * stream.sample_rate()));
		}
		else {
			stream.set_loop_start(int(start_point.count() * stream.sample_rate()));
			stream.set_loop_end(int(end_point.count() * stream.sample_rate()));
		}
		unlock_audio_mutex();
	}
	else {
		const ALuint buffer_id{ALuint(buffer())};

		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer_id, AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer_id, AL_CHANNELS, &channels);

		const std::array<ALint, 2> loop_points{ALint(start_point.count() * sample_rate * channels),
											   ALint(end_point.count() * sample_rate * channels)};
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
		TR_AL_CALL(alBufferiv, buffer_id, AL_LOOP_POINTS_SOFT, loop_points.data());
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, buffer_id);
	}
}

void tr::audio::owning_source::set_looping(bool value)
{
	if (m_stream.has_value()) {
		lock_audio_mutex();
		m_stream->stream->set_looping(value);
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcei, m_id, AL_LOOPING, value);
	}
}

//

tr::audio::buffer::id tr::audio::owning_source::buffer() const
{
	ALint id;
	TR_AL_CALL(alGetSourcei, m_id, AL_BUFFER, &id);
	return audio::buffer::id(id);
}

//

void tr::audio::owning_source::lock_audio_mutex() const
{
	if (m_mutex_refc++ == 0) {
		g_manager.lock_mutex();
	}
}

void tr::audio::owning_source::unlock_audio_mutex() const
{
	if (--m_mutex_refc == 0) {
		g_manager.unlock_mutex();
	}
}

////////////////////////////////////////////////////////////////// SOURCE /////////////////////////////////////////////////////////////////

std::optional<tr::audio::source> tr::audio::try_allocating_source(int priority)
{
	std::shared_ptr<owning_source> base{g_manager.allocate_source(priority)};
	return base != nullptr ? std::optional{source{std::move(base)}} : std::nullopt;
}

tr::audio::source::source(std::shared_ptr<owning_source> base)
	: m_base{base}
{
}

//

void tr::audio::source::use(const buffer& buffer)
{
	m_base->use(buffer);
}

void tr::audio::source::use(std::unique_ptr<stream>&& stream)
{
	m_base->use(std::move(stream));
}

void tr::audio::source::clear()
{
	m_base->clear();
}

//

int tr::audio::source::priority() const
{
	return m_base->priority();
}

const std::bitset<32>& tr::audio::source::classes() const
{
	return m_base->classes();
}

void tr::audio::source::set_classes(const std::bitset<32>& classes)
{
	m_base->set_classes(classes);
}

//

float tr::audio::source::pitch() const
{
	return m_base->pitch();
}

void tr::audio::source::set_pitch(float pitch)
{
	m_base->set_pitch(pitch);
}

void tr::audio::source::set_pitch(float pitch, fsecs time)
{
	g_manager.submit_command(m_base, command::type::pitch, this->pitch(), pitch, duration_cast<duration>(time));
}

//

float tr::audio::source::gain() const
{
	return m_base->gain();
}

void tr::audio::source::set_gain(float gain)
{
	m_base->set_gain(gain);
}

void tr::audio::source::set_gain(float gain, fsecs time)
{
	g_manager.submit_command(m_base, command::type::gain, this->gain(), gain, duration_cast<duration>(time));
}

//

float tr::audio::source::max_distance() const
{
	return m_base->max_distance();
}

void tr::audio::source::set_max_distance(float distance)
{
	m_base->set_max_distance(distance);
}

void tr::audio::source::set_max_distance(float distance, fsecs time)
{
	g_manager.submit_command(m_base, command::type::max_distance, max_distance(), distance, duration_cast<duration>(time));
}

//

float tr::audio::source::rolloff_factor() const
{
	return m_base->rolloff_factor();
}

void tr::audio::source::set_rolloff_factor(float rolloff)
{
	m_base->set_rolloff_factor(rolloff);
}

void tr::audio::source::set_rolloff_factor(float rolloff, fsecs time)
{
	g_manager.submit_command(m_base, command::type::rolloff, rolloff_factor(), rolloff, duration_cast<duration>(time));
}

//

float tr::audio::source::reference_distance() const
{
	return m_base->reference_distance();
}

void tr::audio::source::set_reference_distance(float distance)
{
	m_base->set_reference_distance(distance);
}

void tr::audio::source::set_reference_distance(float distance, fsecs time)
{
	g_manager.submit_command(m_base, command::type::reference_distance, reference_distance(), distance, duration_cast<duration>(time));
}

//

float tr::audio::source::gain_outside_cone() const
{
	return m_base->gain_outside_cone();
}

void tr::audio::source::set_gain_outside_cone(float gain)
{
	m_base->set_gain_outside_cone(gain);
}

void tr::audio::source::set_gain_outside_cone(float gain, fsecs time)
{
	g_manager.submit_command(m_base, command::type::out_cone_gain, gain_outside_cone(), gain, duration_cast<duration>(time));
}

//

tr::angle tr::audio::source::inner_cone_width() const
{
	return m_base->inner_cone_width();
}

tr::angle tr::audio::source::outer_cone_width() const
{
	return m_base->outer_cone_width();
}

void tr::audio::source::set_cone_widths(angle inner, angle outer)
{
	m_base->set_cone_widths(inner, outer);
}

void tr::audio::source::set_cone_widths(angle inner, angle outer, fsecs time)
{
	g_manager.submit_command(m_base, command::type::cone_width, glm::vec2{inner_cone_width().rads(), outer_cone_width().rads()},
							 glm::vec2{inner.rads(), outer.rads()}, duration_cast<duration>(time));
}

//

glm::vec3 tr::audio::source::position() const
{
	return m_base->position();
}

void tr::audio::source::set_position(const glm::vec3& pos)
{
	m_base->set_position(pos);
}

void tr::audio::source::set_position(const glm::vec3& pos, fsecs time)
{
	g_manager.submit_command(m_base, command::type::position, position(), pos, duration_cast<duration>(time));
}

//

glm::vec3 tr::audio::source::velocity() const
{
	return m_base->velocity();
}

void tr::audio::source::set_velocity(const glm::vec3& vel)
{
	m_base->set_velocity(vel);
}

void tr::audio::source::set_velocity(const glm::vec3& vel, fsecs time)
{
	g_manager.submit_command(m_base, command::type::velocity, velocity(), vel, duration_cast<duration>(time));
}

//

glm::vec3 tr::audio::source::direction() const
{
	return m_base->direction();
}

void tr::audio::source::set_direction(const glm::vec3& dir)
{
	m_base->set_direction(dir);
}

void tr::audio::source::set_direction(const glm::vec3& dir, fsecs time)
{
	g_manager.submit_command(m_base, command::type::direction, direction(), dir, duration_cast<duration>(time));
}

//

tr::audio::origin tr::audio::source::origin() const
{
	return m_base->origin();
}

void tr::audio::source::set_origin(audio::origin type)
{
	m_base->set_origin(type);
}

//

tr::audio::state tr::audio::source::state() const
{
	return m_base->state();
}

void tr::audio::source::play()
{
	m_base->play();
}

void tr::audio::source::pause()
{
	return m_base->pause();
}

void tr::audio::source::stop()
{
	m_base->stop();
}

//

tr::fsecs tr::audio::source::length() const
{
	return m_base->length();
}

tr::fsecs tr::audio::source::offset() const
{
	return m_base->offset();
}

void tr::audio::source::set_offset(fsecs offset)
{
	m_base->set_offset(offset);
}

//

bool tr::audio::source::looping() const
{
	return m_base->looping();
}

tr::fsecs tr::audio::source::loop_start() const
{
	return m_base->loop_start();
}

tr::fsecs tr::audio::source::loop_end() const
{
	return m_base->loop_end();
}

void tr::audio::source::set_loop_points(fsecs start_point, fsecs end_point)
{
	m_base->set_loop_points(start_point, end_point);
}

void tr::audio::source::set_looping(bool value)
{
	m_base->set_looping(value);
}