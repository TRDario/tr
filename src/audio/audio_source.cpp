#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/audio_system.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include <AL/alext.h>

constexpr std::size_t AUDIO_STREAM_BUFFER_SIZE{16384};

//

unsigned int tr::base_audio_source::buffer() const
{
	ALint buffer_id;
	TR_AL_CALL(alGetSourcei, id, AL_BUFFER, &buffer_id);
	return static_cast<unsigned int>(buffer_id);
}

void tr::base_audio_source::lock_audio_mutex() const
{
	if (audio_mutex_refc++ == 0) {
		audio_mutex.lock();
	}
}

void tr::base_audio_source::unlock_audio_mutex() const
{
	if (--audio_mutex_refc == 0) {
		audio_mutex.unlock();
	}
}

//

tr::base_audio_source::base_audio_source(int priority)
	: base_gain{1.0f}, priority_{priority}, audio_mutex_refc{0}
{
	TR_AL_CALL(alGenSources, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio source allocation"};
	}
}

tr::audio_source::audio_source(int priority)
{
	TR_ASSERT(audio_system::can_allocate_audio_source(priority), "Tried to allocate more than 128 audio sources at the same time.");

	std::lock_guard lock{audio_mutex};
	if (audio_sources.size() == max_audio_sources) {
		auto it{std::ranges::find_if(audio_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
		audio_sources.erase(it);
	}
	auto it{std::ranges::find_if(audio_sources, [&](auto& s) { return s->priority() < priority; })};
	it = audio_sources.emplace(it, std::make_shared<base_audio_source>(priority));
	if (!audio_thread.joinable()) {
		audio_thread = std::jthread{audio_thread_loop};
	}
	base = *it;
}

tr::base_audio_source::~base_audio_source()
{
	TR_AL_CALL(alDeleteSources, 1, &id);
}

tr::buffer_stream_buffer::buffer_stream_buffer()
	: start_offset{}
{
	TR_AL_CALL(alGenBuffers, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

tr::buffer_stream_buffer::~buffer_stream_buffer()
{
	TR_AL_CALL(alDeleteBuffers, 1, &id);
}

void tr::buffer_stream_buffer::refill(buffer_stream& buffer_stream)
{
	std::array<std::int16_t, AUDIO_STREAM_BUFFER_SIZE> buffer;

	start_offset = buffer_stream.stream->tell();
	const std::span<const std::int16_t> used_buffer{buffer_stream.stream->read(buffer)};
	const ALenum format{buffer_stream.stream->channels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16};
	const ALsizei size{static_cast<ALsizei>(used_buffer.size_bytes()) - static_cast<ALsizei>(used_buffer.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, id, format, used_buffer.data(), size, buffer_stream.stream->sample_rate());
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer reallocation"};
	}
}

//

void tr::base_audio_source::use(const audio_buffer& buffer)
{
	clear();
	TR_AL_CALL(alSourcei, id, AL_BUFFER, buffer.id.get());
	ALint channels;
	TR_AL_CALL(alGetBufferi, buffer.id.get(), AL_CHANNELS, &channels);
	TR_AL_CALL(alSourcei, id, AL_DIRECT_CHANNELS_SOFT, channels == 2);
}

void tr::audio_source::use(const audio_buffer& buffer)
{
	base->use(buffer);
}

void tr::base_audio_source::use(std::unique_ptr<audio_stream>&& stream)
{
	lock_audio_mutex();
	clear();
	this->stream.emplace(std::move(stream));
	unlock_audio_mutex();
}

void tr::audio_source::use(std::unique_ptr<audio_stream>&& stream)
{
	base->use(std::move(stream));
}

void tr::base_audio_source::clear()
{
	lock_audio_mutex();
	stop();
	if (stream.has_value()) {
		TR_AL_CALL(alSourcei, id, AL_BUFFER, 0);
		stream.reset();
	}
	else if (buffer() != 0) {
		set_loop_points(START, END);
		TR_AL_CALL(alSourcei, id, AL_LOOPING, 0);
		TR_AL_CALL(alSourcei, id, AL_BUFFER, 0);
	}
	unlock_audio_mutex();
}

void tr::audio_source::clear()
{
	base->clear();
}

//

int tr::base_audio_source::priority() const
{
	return priority_;
}

int tr::audio_source::priority() const
{
	return base->priority();
}

const std::bitset<32>& tr::base_audio_source::classes() const
{
	return class_flags;
}

const std::bitset<32>& tr::audio_source::classes() const
{
	return base->classes();
}

void tr::base_audio_source::set_classes(const std::bitset<32>& classes)
{
	class_flags = classes;
	set_gain(gain());
}

void tr::audio_source::set_classes(const std::bitset<32>& classes)
{
	base->set_classes(classes);
}

//

float tr::base_audio_source::pitch() const
{
	float pitch;
	TR_AL_CALL(alGetSourcef, id, AL_PITCH, &pitch);
	return pitch;
}

float tr::audio_source::pitch() const
{
	return base->pitch();
}

void tr::base_audio_source::set_pitch(float pitch)
{
	TR_AL_CALL(alSourcef, id, AL_PITCH, std::clamp(pitch, 0.5f, 2.0f));
}

void tr::audio_source::set_pitch(float pitch)
{
	base->set_pitch(pitch);
}

void tr::audio_source::set_pitch(float pitch, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::PITCH, this->pitch(), pitch, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

float tr::base_audio_source::gain() const
{
	return base_gain;
}

float tr::audio_source::gain() const
{
	return base->gain();
}

void tr::base_audio_source::set_gain(float gain)
{
	base_gain = gain;
	for (int i = 0; i < 32; ++i) {
		if (class_flags[i]) {
			gain *= audio_gains[i];
		}
	}
	TR_AL_CALL(alSourcef, id, AL_GAIN, std::max(gain, 0.0f));
}

void tr::audio_source::set_gain(float gain)
{
	base->set_gain(gain);
}

void tr::audio_source::set_gain(float gain, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::GAIN, this->gain(), gain, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

float tr::base_audio_source::max_dist() const
{
	float max_dist;
	TR_AL_CALL(alGetSourcef, id, AL_MAX_DISTANCE, &max_dist);
	return max_dist;
}

float tr::audio_source::max_dist() const
{
	return base->max_dist();
}

void tr::base_audio_source::set_max_dist(float max_dist)
{
	TR_AL_CALL(alSourcef, id, AL_MAX_DISTANCE, std::max(max_dist, 0.0f));
}

void tr::audio_source::set_max_dist(float max_dist)
{
	base->set_max_dist(max_dist);
}

void tr::audio_source::set_max_dist(float max_dist, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::MAX_DIST, this->max_dist(), max_dist, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

float tr::base_audio_source::rolloff() const
{
	float rolloff;
	TR_AL_CALL(alGetSourcef, id, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}

float tr::audio_source::rolloff() const
{
	return base->rolloff();
}

void tr::base_audio_source::set_rolloff(float rolloff)
{
	TR_AL_CALL(alSourcef, id, AL_ROLLOFF_FACTOR, std::max(rolloff, 0.0f));
}

void tr::audio_source::set_rolloff(float rolloff)
{
	base->set_rolloff(rolloff);
}

void tr::audio_source::set_rolloff(float rolloff, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::ROLLOFF, this->rolloff(), rolloff, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

float tr::base_audio_source::ref_dist() const
{
	float ref_dist;
	TR_AL_CALL(alGetSourcef, id, AL_REFERENCE_DISTANCE, &ref_dist);
	return ref_dist;
}

float tr::audio_source::ref_dist() const
{
	return base->ref_dist();
}

void tr::base_audio_source::set_ref_dist(float ref_dist)
{
	TR_AL_CALL(alSourcef, id, AL_REFERENCE_DISTANCE, std::max(ref_dist, 0.0f));
}

void tr::audio_source::set_ref_dist(float ref_dist)
{
	base->set_ref_dist(ref_dist);
}

void tr::audio_source::set_ref_dist(float ref_dist, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::REF_DIST, this->ref_dist(), ref_dist, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

float tr::base_audio_source::out_cone_gain() const
{
	float out_gain;
	TR_AL_CALL(alGetSourcef, id, AL_CONE_OUTER_GAIN, &out_gain);
	return out_gain;
}

float tr::audio_source::out_cone_gain() const
{
	return base->out_cone_gain();
}

void tr::base_audio_source::set_out_cone_gain(float out_cone_gain)
{
	TR_AL_CALL(alSourcef, id, AL_CONE_OUTER_GAIN, std::clamp(out_cone_gain, 0.0f, 1.0f));
}

void tr::audio_source::set_out_cone_gain(float out_cone_gain)
{
	base->set_out_cone_gain(out_cone_gain);
}

void tr::audio_source::set_out_cone_gain(float out_cone_gain, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::OUT_CONE_GAIN, this->out_cone_gain(), out_cone_gain,
								duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

tr::angle tr::base_audio_source::in_cone_w() const
{
	float in_cone_w;
	TR_AL_CALL(alGetSourcef, id, AL_CONE_INNER_ANGLE, &in_cone_w);
	return degs(in_cone_w);
}

tr::angle tr::audio_source::in_cone_w() const
{
	return base->in_cone_w();
}

tr::angle tr::base_audio_source::out_cone_w() const
{
	float out_cone_w;
	TR_AL_CALL(alGetSourcef, id, AL_CONE_OUTER_ANGLE, &out_cone_w);
	return degs(out_cone_w);
}

tr::angle tr::audio_source::out_cone_w() const
{
	return base->out_cone_w();
}

void tr::base_audio_source::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w)
{
	in_cone_w = std::clamp(in_cone_w, 0_deg, 360_deg);
	out_cone_w = std::clamp(out_cone_w, 0_deg, 360_deg);
	TR_ASSERT(in_cone_w < out_cone_w, "Tried to set audio source outer cone as thinner than inner cone (inner: {:d}, outer: {:d}).",
			  in_cone_w, out_cone_w);

	TR_AL_CALL(alSourcef, id, AL_CONE_INNER_ANGLE, in_cone_w.degs());
	TR_AL_CALL(alSourcef, id, AL_CONE_OUTER_ANGLE, out_cone_w.degs());
}

void tr::audio_source::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w)
{
	base->set_cone_w(in_cone_w, out_cone_w);
}

void tr::audio_source::set_cone_w(tr::angle in_cone_w, tr::angle out_cone_w, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::CONE_W, glm::vec2{this->in_cone_w().rads(), this->out_cone_w().rads()},
								glm::vec2{in_cone_w.rads(), out_cone_w.rads()}, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

glm::vec3 tr::base_audio_source::pos() const
{
	glm::vec3 pos;
	TR_AL_CALL(alGetSourcefv, id, AL_POSITION, value_ptr(pos));
	return pos;
}

glm::vec3 tr::audio_source::pos() const
{
	return base->pos();
}

void tr::base_audio_source::set_pos(const glm::vec3& pos)
{
	TR_AL_CALL(alSourcefv, id, AL_POSITION, value_ptr(pos));
}

void tr::audio_source::set_pos(const glm::vec3& pos)
{
	base->set_pos(pos);
}

void tr::audio_source::set_pos(const glm::vec3& pos, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::POS, this->pos(), pos, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

glm::vec3 tr::base_audio_source::vel() const
{
	glm::vec3 vel;
	TR_AL_CALL(alGetSourcefv, id, AL_VELOCITY, value_ptr(vel));
	return vel;
}

glm::vec3 tr::audio_source::vel() const
{
	return base->vel();
}

void tr::base_audio_source::set_vel(const glm::vec3& vel)
{
	TR_AL_CALL(alSourcefv, id, AL_VELOCITY, value_ptr(vel));
}

void tr::audio_source::set_vel(const glm::vec3& vel)
{
	base->set_vel(vel);
}

void tr::audio_source::set_vel(const glm::vec3& vel, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::VEL, this->vel(), vel, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

glm::vec3 tr::base_audio_source::dir() const
{
	glm::vec3 dir;
	TR_AL_CALL(alGetSourcefv, id, AL_DIRECTION, value_ptr(dir));
	return dir;
}

glm::vec3 tr::audio_source::dir() const
{
	return base->dir();
}

void tr::base_audio_source::set_dir(const glm::vec3& dir)
{
	TR_AL_CALL(alSourcefv, id, AL_DIRECTION, value_ptr(dir));
}

void tr::audio_source::set_dir(const glm::vec3& dir)
{
	base->set_dir(dir);
}

void tr::audio_source::set_dir(const glm::vec3& dir, fsecs time)
{
	base->lock_audio_mutex();
	audio_commands.emplace_back(base, audio_command::command_type::DIR, this->dir(), dir, duration_cast<duration>(time));
	base->unlock_audio_mutex();
}

//

tr::audio_origin tr::base_audio_source::origin() const
{
	ALint origin;
	TR_AL_CALL(alGetSourcei, id, AL_SOURCE_RELATIVE, &origin);
	return static_cast<audio_origin>(origin);
}

tr::audio_origin tr::audio_source::origin() const
{
	return base->origin();
}

void tr::base_audio_source::set_origin(audio_origin type)
{
	TR_AL_CALL(alSourcei, id, AL_SOURCE_RELATIVE, static_cast<ALint>(type));
}

void tr::audio_source::set_origin(audio_origin type)
{
	base->set_origin(type);
}

//

tr::audio_state tr::base_audio_source::state() const
{
	ALint state;
	TR_AL_CALL(alGetSourcei, id, AL_SOURCE_STATE, &state);
	switch (state) {
	case AL_INITIAL:
		return audio_state::INITIAL;
	case AL_PLAYING:
		return audio_state::PLAYING;
	case AL_PAUSED:
		return audio_state::PAUSED;
	case AL_STOPPED:
		return audio_state::STOPPED;
	default:
		return audio_state::INITIAL;
	}
}

tr::audio_state tr::audio_source::state() const
{
	return base->state();
}

void tr::base_audio_source::play()
{
	lock_audio_mutex();
	if (stream.has_value()) {
		if (state() == tr::audio_state::INITIAL || state() == tr::audio_state::STOPPED) {
			TR_AL_CALL(alSourcei, id, AL_BUFFER, 0);
			for (auto& buffer : stream->buffers) {
				buffer.refill(*stream);
				TR_AL_CALL(alSourceQueueBuffers, id, 1, &buffer.id);
				if (stream->stream->tell() == stream->stream->length()) {
					break;
				}
			}
		}
	}
	TR_AL_CALL(alSourcePlay, id);
	unlock_audio_mutex();
}

void tr::audio_source::play()
{
	base->play();
}

void tr::base_audio_source::pause()
{
	TR_AL_CALL(alSourcePause, id);
}

void tr::audio_source::pause()
{
	return base->pause();
}

void tr::base_audio_source::stop()
{
	if (stream.has_value()) {
		lock_audio_mutex();
		TR_AL_CALL(alSourceStop, id);
		stream->stream->seek(stream->stream->loop_start());
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourceStop, id);
	}
}

void tr::audio_source::stop()
{
	base->stop();
}

//

tr::fsecs tr::base_audio_source::length() const
{
	if (stream.has_value()) {
		return fsecs{static_cast<float>(stream->stream->length()) / stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate, size;
		TR_AL_CALL(alGetBufferi, id, AL_FREQUENCY, &sample_rate);
		TR_AL_CALL(alGetBufferi, id, AL_SIZE, &size);
		return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size) / sample_rate};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::length() const
{
	return base->length();
}

tr::fsecs tr::base_audio_source::offset() const
{
	float offset;
	TR_AL_CALL(alGetSourcef, id, AL_SEC_OFFSET, &offset);

	if (stream.has_value()) {
		lock_audio_mutex();
		const tr::audio_state state{this->state()};
		if (state == tr::audio_state::INITIAL || state == tr::audio_state::STOPPED) {
			return fsecs{stream->stream->tell() / static_cast<float>(stream->stream->sample_rate())};
		}

		ALint buf_id;
		TR_AL_CALL(alGetSourcei, id, AL_BUFFER, &buf_id);

		auto& buf{*std::ranges::find(stream->buffers, static_cast<unsigned int>(buf_id), &buffer_stream_buffer::id)};
		unlock_audio_mutex();
		return fsecs{buf.start_offset / static_cast<float>(stream->stream->sample_rate()) + offset};
	}
	else {
		return fsecs{offset};
	}
}

tr::fsecs tr::audio_source::offset() const
{
	return base->offset();
}

void tr::base_audio_source::set_offset(fsecs offset)
{
	if (stream.has_value()) {
		lock_audio_mutex();
		audio_state state{this->state()};
		stream->stream->seek(static_cast<int>(offset.count() * stream->stream->sample_rate()));
		TR_AL_CALL(alSourceStop, id);
		switch (state) {
		case audio_state::PLAYING:
			play();
			break;
		case audio_state::PAUSED:
			play();
			pause();
			break;
		case audio_state::INITIAL:
		case audio_state::STOPPED:
			break;
		}
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcef, id, AL_SEC_OFFSET, offset.count());
	}

	TR_AL_CALL(alSourcef, id, AL_SEC_OFFSET, offset.count());
}

void tr::audio_source::set_offset(fsecs offset)
{
	base->set_offset(offset);
}

//

bool tr::base_audio_source::looping() const
{
	if (stream.has_value()) {
		return stream->stream->looping();
	}
	else {
		ALint looping;
		TR_AL_CALL(alGetSourcei, id, AL_LOOPING, &looping);
		return looping;
	}
}

bool tr::audio_source::looping() const
{
	return base->looping();
}

tr::fsecs tr::base_audio_source::loop_start() const
{
	if (stream.has_value()) {
		return fsecs{static_cast<float>(stream->stream->loop_start()) / stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[0]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::loop_start() const
{
	return base->loop_start();
}

tr::fsecs tr::base_audio_source::loop_end() const
{
	if (stream.has_value()) {
		return fsecs{static_cast<float>(stream->stream->loop_end()) / stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[1]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::loop_end() const
{
	return base->loop_end();
}

void tr::base_audio_source::set_loop_points(fsecs start, fsecs end)
{
	if (length() == fsecs::zero()) {
		return;
	}

	start = std::clamp(start, START, length());
	end = std::clamp(end, START, length());
	TR_ASSERT(start < end, "Tried to set audio source loop end before start (start: {}s, end: {}s).", start.count(), end.count());

	if (stream.has_value()) {
		lock_audio_mutex();
		if (start >= loop_end()) {
			stream->stream->set_loop_end(static_cast<int>(end.count() * stream->stream->sample_rate()));
			stream->stream->set_loop_start(static_cast<int>(start.count() * stream->stream->sample_rate()));
		}
		else {
			stream->stream->set_loop_start(static_cast<int>(start.count() * stream->stream->sample_rate()));
			stream->stream->set_loop_end(static_cast<int>(end.count() * stream->stream->sample_rate()));
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
		TR_AL_CALL(alSourcei, id, AL_BUFFER, 0);
		TR_AL_CALL(alBufferiv, buffer, AL_LOOP_POINTS_SOFT, loop_points.data());
		TR_AL_CALL(alSourcei, id, AL_BUFFER, buffer);
	}
}

void tr::audio_source::set_loop_points(fsecs start, fsecs end)
{
	base->set_loop_points(start, end);
}

void tr::base_audio_source::set_looping(bool value)
{
	if (stream.has_value()) {
		lock_audio_mutex();
		stream->stream->set_looping(value);
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcei, id, AL_LOOPING, value);
	}
}

void tr::audio_source::set_looping(bool value)
{
	base->set_looping(value);
}