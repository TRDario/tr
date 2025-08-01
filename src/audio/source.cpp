#include "../../include/tr/audio/source.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include <AL/alext.h>

constexpr std::size_t AUDIO_STREAM_BUFFER_SIZE{16384};

//

unsigned int tr::audio::source_base::buffer() const
{
	ALint id;
	TR_AL_CALL(alGetSourcei, m_id, AL_BUFFER, &id);
	return static_cast<unsigned int>(id);
}

void tr::audio::source_base::lock_audio_mutex() const
{
	if (m_mutex_refc++ == 0) {
		mutex.lock();
	}
}

void tr::audio::source_base::unlock_audio_mutex() const
{
	if (--m_mutex_refc == 0) {
		mutex.unlock();
	}
}

//

tr::audio::source_base::source_base(int priority)
	: m_gain{1.0f}, m_priority{priority}, m_mutex_refc{0}
{
	TR_AL_CALL(alGenSources, 1, &m_id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio source allocation"};
	}
}

tr::audio::source::source(int priority)
{
	TR_ASSERT(can_allocate_source(priority), "Tried to allocate more than the maximum allowed number of sources at the same time.");

	std::lock_guard lock{mutex};
	if (sources.size() == max_sources) {
		auto it{std::ranges::find_if(sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
		sources.erase(it);
	}
	auto it{std::ranges::find_if(sources, [&](auto& s) { return s->priority() < priority; })};
	it = sources.emplace(it, std::make_shared<source_base>(priority));
	if (!thread.joinable()) {
		thread = std::jthread{thread_fn};
	}
	m_base = *it;
}

tr::audio::source_base::~source_base()
{
	TR_AL_CALL(alDeleteSources, 1, &m_id);
}

tr::audio::buffer_stream_buffer::buffer_stream_buffer()
	: start_offset{}
{
	TR_AL_CALL(alGenBuffers, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

tr::audio::buffer_stream_buffer::~buffer_stream_buffer()
{
	TR_AL_CALL(alDeleteBuffers, 1, &id);
}

void tr::audio::refill(stream& stream, buffer_stream_buffer& buffer)
{
	std::array<std::int16_t, AUDIO_STREAM_BUFFER_SIZE> data;

	buffer.start_offset = stream.tell();
	const std::span<const std::int16_t> used_data{stream.read(data)};
	const ALenum format{stream.channels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16};
	const ALsizei size{static_cast<ALsizei>(used_data.size_bytes()) - static_cast<ALsizei>(used_data.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, buffer.id, format, used_data.data(), size, stream.sample_rate());
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer reallocation"};
	}
}

//

void tr::audio::source_base::use(const audio::buffer& buffer)
{
	clear();
	TR_AL_CALL(alSourcei, m_id, AL_BUFFER, buffer.m_id.get());
	ALint channels;
	TR_AL_CALL(alGetBufferi, buffer.m_id.get(), AL_CHANNELS, &channels);
	TR_AL_CALL(alSourcei, m_id, AL_DIRECT_CHANNELS_SOFT, channels == 2);
}

void tr::audio::source::use(const buffer& buffer)
{
	m_base->use(buffer);
}

void tr::audio::source_base::use(std::unique_ptr<stream>&& stream)
{
	lock_audio_mutex();
	clear();
	this->m_stream.emplace(std::move(stream));
	unlock_audio_mutex();
}

void tr::audio::source::use(std::unique_ptr<stream>&& stream)
{
	m_base->use(std::move(stream));
}

void tr::audio::source_base::clear()
{
	lock_audio_mutex();
	stop();
	if (m_stream.has_value()) {
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
		m_stream.reset();
	}
	else if (buffer() != 0) {
		set_loop_points(START, END);
		TR_AL_CALL(alSourcei, m_id, AL_LOOPING, 0);
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
	}
	unlock_audio_mutex();
}

void tr::audio::source::clear()
{
	m_base->clear();
}

//

int tr::audio::source_base::priority() const
{
	return m_priority;
}

int tr::audio::source::priority() const
{
	return m_base->priority();
}

const std::bitset<32>& tr::audio::source_base::classes() const
{
	return m_classes;
}

const std::bitset<32>& tr::audio::source::classes() const
{
	return m_base->classes();
}

void tr::audio::source_base::set_classes(const std::bitset<32>& classes)
{
	m_classes = classes;
	set_gain(gain());
}

void tr::audio::source::set_classes(const std::bitset<32>& classes)
{
	m_base->set_classes(classes);
}

//

float tr::audio::source_base::pitch() const
{
	float pitch;
	TR_AL_CALL(alGetSourcef, m_id, AL_PITCH, &pitch);
	return pitch;
}

float tr::audio::source::pitch() const
{
	return m_base->pitch();
}

void tr::audio::source_base::set_pitch(float pitch)
{
	TR_AL_CALL(alSourcef, m_id, AL_PITCH, std::clamp(pitch, 0.5f, 2.0f));
}

void tr::audio::source::set_pitch(float pitch)
{
	m_base->set_pitch(pitch);
}

void tr::audio::source::set_pitch(float pitch, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::PITCH, this->pitch(), pitch, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

float tr::audio::source_base::gain() const
{
	return m_gain;
}

float tr::audio::source::gain() const
{
	return m_base->gain();
}

void tr::audio::source_base::set_gain(float gain)
{
	m_gain = gain;
	for (int i = 0; i < 32; ++i) {
		if (m_classes[i]) {
			gain *= gains[i];
		}
	}
	TR_AL_CALL(alSourcef, m_id, AL_GAIN, std::max(gain, 0.0f));
}

void tr::audio::source::set_gain(float gain)
{
	m_base->set_gain(gain);
}

void tr::audio::source::set_gain(float gain, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::GAIN, this->gain(), gain, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

float tr::audio::source_base::max_dist() const
{
	float max_dist;
	TR_AL_CALL(alGetSourcef, m_id, AL_MAX_DISTANCE, &max_dist);
	return max_dist;
}

float tr::audio::source::max_dist() const
{
	return m_base->max_dist();
}

void tr::audio::source_base::set_max_dist(float max_dist)
{
	TR_AL_CALL(alSourcef, m_id, AL_MAX_DISTANCE, std::max(max_dist, 0.0f));
}

void tr::audio::source::set_max_dist(float max_dist)
{
	m_base->set_max_dist(max_dist);
}

void tr::audio::source::set_max_dist(float max_dist, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::MAX_DIST, this->max_dist(), max_dist, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

float tr::audio::source_base::rolloff() const
{
	float rolloff;
	TR_AL_CALL(alGetSourcef, m_id, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}

float tr::audio::source::rolloff() const
{
	return m_base->rolloff();
}

void tr::audio::source_base::set_rolloff(float rolloff)
{
	TR_AL_CALL(alSourcef, m_id, AL_ROLLOFF_FACTOR, std::max(rolloff, 0.0f));
}

void tr::audio::source::set_rolloff(float rolloff)
{
	m_base->set_rolloff(rolloff);
}

void tr::audio::source::set_rolloff(float rolloff, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::ROLLOFF, this->rolloff(), rolloff, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

float tr::audio::source_base::ref_dist() const
{
	float ref_dist;
	TR_AL_CALL(alGetSourcef, m_id, AL_REFERENCE_DISTANCE, &ref_dist);
	return ref_dist;
}

float tr::audio::source::ref_dist() const
{
	return m_base->ref_dist();
}

void tr::audio::source_base::set_ref_dist(float ref_dist)
{
	TR_AL_CALL(alSourcef, m_id, AL_REFERENCE_DISTANCE, std::max(ref_dist, 0.0f));
}

void tr::audio::source::set_ref_dist(float ref_dist)
{
	m_base->set_ref_dist(ref_dist);
}

void tr::audio::source::set_ref_dist(float ref_dist, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::REF_DIST, this->ref_dist(), ref_dist, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

float tr::audio::source_base::out_cone_gain() const
{
	float out_gain;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_OUTER_GAIN, &out_gain);
	return out_gain;
}

float tr::audio::source::out_cone_gain() const
{
	return m_base->out_cone_gain();
}

void tr::audio::source_base::set_out_cone_gain(float out_cone_gain)
{
	TR_AL_CALL(alSourcef, m_id, AL_CONE_OUTER_GAIN, std::clamp(out_cone_gain, 0.0f, 1.0f));
}

void tr::audio::source::set_out_cone_gain(float out_cone_gain)
{
	m_base->set_out_cone_gain(out_cone_gain);
}

void tr::audio::source::set_out_cone_gain(float out_cone_gain, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::OUT_CONE_GAIN, this->out_cone_gain(), out_cone_gain, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

tr::angle tr::audio::source_base::in_cone_w() const
{
	float in_cone_w;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_INNER_ANGLE, &in_cone_w);
	return degs(in_cone_w);
}

tr::angle tr::audio::source::in_cone_w() const
{
	return m_base->in_cone_w();
}

tr::angle tr::audio::source_base::out_cone_w() const
{
	float out_cone_w;
	TR_AL_CALL(alGetSourcef, m_id, AL_CONE_OUTER_ANGLE, &out_cone_w);
	return degs(out_cone_w);
}

tr::angle tr::audio::source::out_cone_w() const
{
	return m_base->out_cone_w();
}

void tr::audio::source_base::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w)
{
	in_cone_w = std::clamp(in_cone_w, 0_deg, 360_deg);
	out_cone_w = std::clamp(out_cone_w, 0_deg, 360_deg);
	TR_ASSERT(in_cone_w < out_cone_w, "Tried to set audio source outer cone as thinner than inner cone (inner: {:d}, outer: {:d}).",
			  in_cone_w, out_cone_w);

	TR_AL_CALL(alSourcef, m_id, AL_CONE_INNER_ANGLE, in_cone_w.degs());
	TR_AL_CALL(alSourcef, m_id, AL_CONE_OUTER_ANGLE, out_cone_w.degs());
}

void tr::audio::source::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w)
{
	m_base->set_cone_w(in_cone_w, out_cone_w);
}

void tr::audio::source::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::CONE_W, glm::vec2{this->in_cone_w().rads(), this->out_cone_w().rads()},
						  glm::vec2{in_cone_w.rads(), out_cone_w.rads()}, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

glm::vec3 tr::audio::source_base::pos() const
{
	glm::vec3 pos;
	TR_AL_CALL(alGetSourcefv, m_id, AL_POSITION, value_ptr(pos));
	return pos;
}

glm::vec3 tr::audio::source::pos() const
{
	return m_base->pos();
}

void tr::audio::source_base::set_pos(const glm::vec3& pos)
{
	TR_AL_CALL(alSourcefv, m_id, AL_POSITION, value_ptr(pos));
}

void tr::audio::source::set_pos(const glm::vec3& pos)
{
	m_base->set_pos(pos);
}

void tr::audio::source::set_pos(const glm::vec3& pos, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::POS, this->pos(), pos, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

glm::vec3 tr::audio::source_base::vel() const
{
	glm::vec3 vel;
	TR_AL_CALL(alGetSourcefv, m_id, AL_VELOCITY, value_ptr(vel));
	return vel;
}

glm::vec3 tr::audio::source::vel() const
{
	return m_base->vel();
}

void tr::audio::source_base::set_vel(const glm::vec3& vel)
{
	TR_AL_CALL(alSourcefv, m_id, AL_VELOCITY, value_ptr(vel));
}

void tr::audio::source::set_vel(const glm::vec3& vel)
{
	m_base->set_vel(vel);
}

void tr::audio::source::set_vel(const glm::vec3& vel, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::VEL, this->vel(), vel, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

glm::vec3 tr::audio::source_base::dir() const
{
	glm::vec3 dir;
	TR_AL_CALL(alGetSourcefv, m_id, AL_DIRECTION, value_ptr(dir));
	return dir;
}

glm::vec3 tr::audio::source::dir() const
{
	return m_base->dir();
}

void tr::audio::source_base::set_dir(const glm::vec3& dir)
{
	TR_AL_CALL(alSourcefv, m_id, AL_DIRECTION, value_ptr(dir));
}

void tr::audio::source::set_dir(const glm::vec3& dir)
{
	m_base->set_dir(dir);
}

void tr::audio::source::set_dir(const glm::vec3& dir, fsecs time)
{
	m_base->lock_audio_mutex();
	commands.emplace_back(m_base, command::type::DIR, this->dir(), dir, duration_cast<duration>(time));
	m_base->unlock_audio_mutex();
}

//

tr::audio::origin tr::audio::source_base::origin() const
{
	ALint origin;
	TR_AL_CALL(alGetSourcei, m_id, AL_SOURCE_RELATIVE, &origin);
	return static_cast<audio::origin>(origin);
}

tr::audio::origin tr::audio::source::origin() const
{
	return m_base->origin();
}

void tr::audio::source_base::set_origin(audio::origin type)
{
	TR_AL_CALL(alSourcei, m_id, AL_SOURCE_RELATIVE, static_cast<ALint>(type));
}

void tr::audio::source::set_origin(audio::origin type)
{
	m_base->set_origin(type);
}

//

tr::audio::state tr::audio::source_base::state() const
{
	ALint state;
	TR_AL_CALL(alGetSourcei, m_id, AL_SOURCE_STATE, &state);
	switch (state) {
	case AL_INITIAL:
		return state::INITIAL;
	case AL_PLAYING:
		return state::PLAYING;
	case AL_PAUSED:
		return state::PAUSED;
	case AL_STOPPED:
		return state::STOPPED;
	default:
		return state::INITIAL;
	}
}

tr::audio::state tr::audio::source::state() const
{
	return m_base->state();
}

void tr::audio::source_base::play()
{
	lock_audio_mutex();
	if (m_stream.has_value()) {
		if (state() == state::INITIAL || state() == state::STOPPED) {
			TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
			for (auto& buffer : m_stream->buffers) {
				refill(*m_stream->stream, buffer);
				TR_AL_CALL(alSourceQueueBuffers, m_id, 1, &buffer.id);
				if (m_stream->stream->tell() == m_stream->stream->length()) {
					break;
				}
			}
		}
	}
	TR_AL_CALL(alSourcePlay, m_id);
	unlock_audio_mutex();
}

void tr::audio::source::play()
{
	m_base->play();
}

void tr::audio::source_base::pause()
{
	TR_AL_CALL(alSourcePause, m_id);
}

void tr::audio::source::pause()
{
	return m_base->pause();
}

void tr::audio::source_base::stop()
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

void tr::audio::source::stop()
{
	m_base->stop();
}

//

tr::fsecs tr::audio::source_base::length() const
{
	if (m_stream.has_value()) {
		return fsecs{static_cast<float>(m_stream->stream->length()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate, size;
		TR_AL_CALL(alGetBufferi, m_id, AL_FREQUENCY, &sample_rate);
		TR_AL_CALL(alGetBufferi, m_id, AL_SIZE, &size);
		return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size) / sample_rate};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio::source::length() const
{
	return m_base->length();
}

tr::fsecs tr::audio::source_base::offset() const
{
	float offset;
	TR_AL_CALL(alGetSourcef, m_id, AL_SEC_OFFSET, &offset);

	if (m_stream.has_value()) {
		lock_audio_mutex();
		const audio::state state{this->state()};
		if (state == state::INITIAL || state == state::STOPPED) {
			return fsecs{m_stream->stream->tell() / static_cast<float>(m_stream->stream->sample_rate())};
		}

		ALint buf_id;
		TR_AL_CALL(alGetSourcei, m_id, AL_BUFFER, &buf_id);

		auto& buf{*std::ranges::find(m_stream->buffers, static_cast<unsigned int>(buf_id), &buffer_stream_buffer::id)};
		unlock_audio_mutex();
		return fsecs{buf.start_offset / static_cast<float>(m_stream->stream->sample_rate()) + offset};
	}
	else {
		return fsecs{offset};
	}
}

tr::fsecs tr::audio::source::offset() const
{
	return m_base->offset();
}

void tr::audio::source_base::set_offset(fsecs offset)
{
	if (m_stream.has_value()) {
		lock_audio_mutex();
		audio::state state{this->state()};
		m_stream->stream->seek(static_cast<int>(offset.count() * m_stream->stream->sample_rate()));
		TR_AL_CALL(alSourceStop, m_id);
		switch (state) {
		case state::PLAYING:
			play();
			break;
		case state::PAUSED:
			play();
			pause();
			break;
		case state::INITIAL:
		case state::STOPPED:
			break;
		}
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcef, m_id, AL_SEC_OFFSET, offset.count());
	}

	TR_AL_CALL(alSourcef, m_id, AL_SEC_OFFSET, offset.count());
}

void tr::audio::source::set_offset(fsecs offset)
{
	m_base->set_offset(offset);
}

//

bool tr::audio::source_base::looping() const
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

bool tr::audio::source::looping() const
{
	return m_base->looping();
}

tr::fsecs tr::audio::source_base::loop_start() const
{
	if (m_stream.has_value()) {
		return fsecs{static_cast<float>(m_stream->stream->loop_start()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, m_id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[0]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio::source::loop_start() const
{
	return m_base->loop_start();
}

tr::fsecs tr::audio::source_base::loop_end() const
{
	if (m_stream.has_value()) {
		return fsecs{static_cast<float>(m_stream->stream->loop_end()) / m_stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, m_id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[1]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio::source::loop_end() const
{
	return m_base->loop_end();
}

void tr::audio::source_base::set_loop_points(fsecs start, fsecs end)
{
	if (length() == fsecs::zero()) {
		return;
	}

	start = std::clamp(start, START, length());
	end = std::clamp(end, START, length());
	TR_ASSERT(start < end, "Tried to set audio source loop end before start (start: {}s, end: {}s).", start.count(), end.count());

	if (m_stream.has_value()) {
		lock_audio_mutex();
		if (start >= loop_end()) {
			m_stream->stream->set_loop_end(static_cast<int>(end.count() * m_stream->stream->sample_rate()));
			m_stream->stream->set_loop_start(static_cast<int>(start.count() * m_stream->stream->sample_rate()));
		}
		else {
			m_stream->stream->set_loop_start(static_cast<int>(start.count() * m_stream->stream->sample_rate()));
			m_stream->stream->set_loop_end(static_cast<int>(end.count() * m_stream->stream->sample_rate()));
		}
		unlock_audio_mutex();
	}
	else {
		const ALuint buffer{this->buffer()};

		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer, AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer, AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points{static_cast<ALint>(start.count() * sample_rate * channels),
										 static_cast<ALint>(end.count() * sample_rate * channels)};
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, 0);
		TR_AL_CALL(alBufferiv, buffer, AL_LOOP_POINTS_SOFT, loop_points.data());
		TR_AL_CALL(alSourcei, m_id, AL_BUFFER, buffer);
	}
}

void tr::audio::source::set_loop_points(fsecs start, fsecs end)
{
	m_base->set_loop_points(start, end);
}

void tr::audio::source_base::set_looping(bool value)
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

void tr::audio::source::set_looping(bool value)
{
	m_base->set_looping(value);
}

//

bool tr::audio::can_allocate_source(int priority)
{
	if (sources.size() < max_sources) {
		return true;
	}
	const auto it{std::ranges::find_if(sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
	return it != sources.end();
}

std::optional<tr::audio::source> tr::audio::try_allocating_source(int priority)
{
	return can_allocate_source(priority) ? std::optional<tr::audio::source>{priority} : std::nullopt;
}