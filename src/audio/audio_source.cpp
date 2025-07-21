#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/audio_system.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include <AL/alext.h>

constexpr std::size_t AUDIO_STREAM_BUFFER_SIZE{16384};

//

unsigned int tr::_audio_source::buffer() const noexcept
{
	ALint id;
	TR_AL_CALL(alGetSourcei, _id, AL_BUFFER, &id);
	return static_cast<unsigned int>(id);
}

void tr::_audio_source::lock_audio_mutex() const noexcept
{
	if (_audio_mutex_refc++ == 0) {
		try {
			_audio_mutex.lock();
		}
		catch (std::system_error&) {
			terminate("System error", "Exception occurred while locking the audio mutex.");
		}
	}
}

void tr::_audio_source::unlock_audio_mutex() const noexcept
{
	if (--_audio_mutex_refc == 0) {
		try {
			_audio_mutex.unlock();
		}
		catch (std::system_error&) {
			terminate("System error", "Exception occurred while unlocking the audio mutex.");
		}
	}
}

//

tr::_audio_source::_audio_source(int priority)
	: _gain{1.0f}, _priority{priority}, _audio_mutex_refc{0}
{
	TR_AL_CALL(alGenSources, 1, &_id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		terminate("Out of memory", "Exception occurred while creating an audio source.");
	}
}

tr::audio_source::audio_source(int priority) noexcept
{
	TR_ASSERT(audio_system::can_allocate_audio_source(priority), "Tried to allocate more than 128 audio sources at the same time.");

	try {
		std::lock_guard lock{_audio_mutex};
		if (_audio_sources.size() == _max_audio_sources) {
			auto it{std::ranges::find_if(_audio_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
			_audio_sources.erase(it);
		}
		auto it{std::ranges::find_if(_audio_sources, [&](auto& s) { return s->priority() < priority; })};
		it = _audio_sources.emplace(it, std::make_shared<_audio_source>(priority));
		if (!_audio_thread.joinable()) {
			_audio_thread = std::jthread{_audio_thread_loop};
		}
		_impl = *it;
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while creating an audio source.");
	}
	catch (std::system_error&) {
		terminate("System error", "Exception occurred while creating an audio source.");
	}
}

tr::_audio_source::~_audio_source() noexcept
{
	TR_AL_CALL(alDeleteSources, 1, &_id);
}

void tr::_buffer_stream_buffer::refill(_buffer_stream& stream) noexcept
{
	try {
		static std::vector<std::int16_t> data_buf(AUDIO_STREAM_BUFFER_SIZE);
		start_offset = stream.stream->tell();
		data_buf.resize(stream.stream->read(data_buf.begin(), AUDIO_STREAM_BUFFER_SIZE));
		TR_AL_CALL(alBufferData, id, stream.stream->channels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data_buf.data(),
				   static_cast<ALsizei>(data_buf.size() * sizeof(std::int16_t)), stream.stream->sample_rate());
		if (alGetError() == AL_OUT_OF_MEMORY) {
			terminate("Out of memory", "Exception occurred while refilling an audio buffer.");
		}
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while refilling an audio buffer.");
	}
}

//

void tr::_audio_source::use(const audio_buffer& buffer) noexcept
{
	clear();
	TR_AL_CALL(alSourcei, _id, AL_BUFFER, buffer._id.get());
	ALint channels;
	TR_AL_CALL(alGetBufferi, buffer._id.get(), AL_CHANNELS, &channels);
	TR_AL_CALL(alSourcei, _id, AL_DIRECT_CHANNELS_SOFT, channels == 2);
}

void tr::audio_source::use(const audio_buffer& buffer) noexcept
{
	_impl->use(buffer);
}

void tr::_audio_source::use(std::unique_ptr<audio_stream>&& stream)
{
	lock_audio_mutex();
	clear();
	_stream.emplace(std::move(stream));
	unlock_audio_mutex();
}

void tr::audio_source::use(std::unique_ptr<audio_stream>&& stream) noexcept
{
	_impl->use(std::move(stream));
}

void tr::_audio_source::clear() noexcept
{
	lock_audio_mutex();
	stop();
	if (_stream.has_value()) {
		TR_AL_CALL(alSourcei, _id, AL_BUFFER, 0);
		_stream.reset();
	}
	else if (buffer() != 0) {
		set_loop_points(START, END);
		TR_AL_CALL(alSourcei, _id, AL_LOOPING, 0);
		TR_AL_CALL(alSourcei, _id, AL_BUFFER, 0);
	}
	unlock_audio_mutex();
}

void tr::audio_source::clear() noexcept
{
	_impl->clear();
}

//

int tr::_audio_source::priority() const noexcept
{
	return _priority;
}

int tr::audio_source::priority() const noexcept
{
	return _impl->priority();
}

const std::bitset<32>& tr::_audio_source::classes() const noexcept
{
	return _classes;
}

const std::bitset<32>& tr::audio_source::classes() const noexcept
{
	return _impl->classes();
}

void tr::_audio_source::set_classes(const std::bitset<32>& classes) noexcept
{
	_classes = classes;
	set_gain(gain());
}

void tr::audio_source::set_classes(const std::bitset<32>& classes) noexcept
{
	_impl->set_classes(classes);
}

//

float tr::_audio_source::pitch() const noexcept
{
	float pitch;
	TR_AL_CALL(alGetSourcef, _id, AL_PITCH, &pitch);
	return pitch;
}

float tr::audio_source::pitch() const noexcept
{
	return _impl->pitch();
}

void tr::_audio_source::set_pitch(float pitch) noexcept
{
	TR_AL_CALL(alSourcef, _id, AL_PITCH, std::clamp(pitch, 0.5f, 2.0f));
}

void tr::audio_source::set_pitch(float pitch) noexcept
{
	_impl->set_pitch(pitch);
}

void tr::audio_source::set_pitch(float pitch, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::PITCH, this->pitch(), pitch, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the pitch of an audio source.");
	}
}

//

float tr::_audio_source::gain() const noexcept
{
	return _gain;
}

float tr::audio_source::gain() const noexcept
{
	return _impl->gain();
}

void tr::_audio_source::set_gain(float gain) noexcept
{
	_gain = gain;
	for (int i = 0; i < 32; ++i) {
		if (_classes[i]) {
			gain *= _audio_gains[i];
		}
	}
	TR_AL_CALL(alSourcef, _id, AL_GAIN, std::max(gain, 0.0f));
}

void tr::audio_source::set_gain(float gain) noexcept
{
	_impl->set_gain(gain);
}

void tr::audio_source::set_gain(float gain, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::GAIN, this->gain(), gain, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the gain of an audio source.");
	}
}

//

float tr::_audio_source::max_dist() const noexcept
{
	float max_dist;
	TR_AL_CALL(alGetSourcef, _id, AL_MAX_DISTANCE, &max_dist);
	return max_dist;
}

float tr::audio_source::max_dist() const noexcept
{
	return _impl->max_dist();
}

void tr::_audio_source::set_max_dist(float max_dist) noexcept
{
	TR_AL_CALL(alSourcef, _id, AL_MAX_DISTANCE, std::max(max_dist, 0.0f));
}

void tr::audio_source::set_max_dist(float max_dist) noexcept
{
	_impl->set_max_dist(max_dist);
}

void tr::audio_source::set_max_dist(float max_dist, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::MAX_DIST, this->max_dist(), max_dist, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the max distance of an audio source.");
	}
}

//

float tr::_audio_source::rolloff() const noexcept
{
	float rolloff;
	TR_AL_CALL(alGetSourcef, _id, AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}

float tr::audio_source::rolloff() const noexcept
{
	return _impl->rolloff();
}

void tr::_audio_source::set_rolloff(float rolloff) noexcept
{
	TR_AL_CALL(alSourcef, _id, AL_ROLLOFF_FACTOR, std::max(rolloff, 0.0f));
}

void tr::audio_source::set_rolloff(float rolloff) noexcept
{
	_impl->set_rolloff(rolloff);
}

void tr::audio_source::set_rolloff(float rolloff, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::ROLLOFF, this->rolloff(), rolloff, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the rolloff of an audio source.");
	}
}

//

float tr::_audio_source::ref_dist() const noexcept
{
	float ref_dist;
	TR_AL_CALL(alGetSourcef, _id, AL_REFERENCE_DISTANCE, &ref_dist);
	return ref_dist;
}

float tr::audio_source::ref_dist() const noexcept
{
	return _impl->ref_dist();
}

void tr::_audio_source::set_ref_dist(float ref_dist) noexcept
{
	TR_AL_CALL(alSourcef, _id, AL_REFERENCE_DISTANCE, std::max(ref_dist, 0.0f));
}

void tr::audio_source::set_ref_dist(float ref_dist) noexcept
{
	_impl->set_ref_dist(ref_dist);
}

void tr::audio_source::set_ref_dist(float ref_dist, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::REF_DIST, this->ref_dist(), ref_dist, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the reference distance of an audio source.");
	}
}

//

float tr::_audio_source::out_cone_gain() const noexcept
{
	float out_gain;
	TR_AL_CALL(alGetSourcef, _id, AL_CONE_OUTER_GAIN, &out_gain);
	return out_gain;
}

float tr::audio_source::out_cone_gain() const noexcept
{
	return _impl->out_cone_gain();
}

void tr::_audio_source::set_out_cone_gain(float out_cone_gain) noexcept
{
	TR_AL_CALL(alSourcef, _id, AL_CONE_OUTER_GAIN, std::clamp(out_cone_gain, 0.0f, 1.0f));
}

void tr::audio_source::set_out_cone_gain(float out_cone_gain) noexcept
{
	_impl->set_out_cone_gain(out_cone_gain);
}

void tr::audio_source::set_out_cone_gain(float out_cone_gain, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::OUT_CONE_GAIN, this->out_cone_gain(), out_cone_gain,
									 duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the outer cone gain of an audio source.");
	}
}

//

tr::fangle tr::_audio_source::in_cone_w() const noexcept
{
	float in_cone_w;
	TR_AL_CALL(alGetSourcef, _id, AL_CONE_INNER_ANGLE, &in_cone_w);
	return degs(in_cone_w);
}

tr::fangle tr::audio_source::in_cone_w() const noexcept
{
	return _impl->in_cone_w();
}

tr::fangle tr::_audio_source::out_cone_w() const noexcept
{
	float out_cone_w;
	TR_AL_CALL(alGetSourcef, _id, AL_CONE_OUTER_ANGLE, &out_cone_w);
	return degs(out_cone_w);
}

tr::fangle tr::audio_source::out_cone_w() const noexcept
{
	return _impl->out_cone_w();
}

void tr::_audio_source::set_cone_w(tr::fangle in_cone_w, tr::fangle out_cone_w) noexcept
{
	in_cone_w = std::clamp(in_cone_w, 0_degf, 360_degf);
	out_cone_w = std::clamp(out_cone_w, 0_degf, 360_degf);
	TR_ASSERT(in_cone_w < out_cone_w, "Tried to set audio source outer cone as thinner than inner cone (inner: {:d}, outer: {:d}).",
			  in_cone_w, out_cone_w);

	TR_AL_CALL(alSourcef, _id, AL_CONE_INNER_ANGLE, in_cone_w.degs());
	TR_AL_CALL(alSourcef, _id, AL_CONE_OUTER_ANGLE, out_cone_w.degs());
}

void tr::audio_source::set_cone_w(tr::fangle in_cone_w, tr::fangle out_cone_w) noexcept
{
	_impl->set_cone_w(in_cone_w, out_cone_w);
}

void tr::audio_source::set_cone_w(tr::fangle in_cone_w, tr::fangle out_cone_w, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::CONE_W, glm::vec2{this->in_cone_w().rads(), this->out_cone_w().rads()},
									 glm::vec2{in_cone_w.rads(), out_cone_w.rads()}, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the cone widths of an audio source.");
	}
}

//

glm::vec3 tr::_audio_source::pos() const noexcept
{
	glm::vec3 pos;
	TR_AL_CALL(alGetSourcefv, _id, AL_POSITION, value_ptr(pos));
	return pos;
}

glm::vec3 tr::audio_source::pos() const noexcept
{
	return _impl->pos();
}

void tr::_audio_source::set_pos(const glm::vec3& pos) noexcept
{
	TR_AL_CALL(alSourcefv, _id, AL_POSITION, value_ptr(pos));
}

void tr::audio_source::set_pos(const glm::vec3& pos) noexcept
{
	_impl->set_pos(pos);
}

void tr::audio_source::set_pos(const glm::vec3& pos, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::POS, this->pos(), pos, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the position of an audio source.");
	}
}

//

glm::vec3 tr::_audio_source::vel() const noexcept
{
	glm::vec3 vel;
	TR_AL_CALL(alGetSourcefv, _id, AL_VELOCITY, value_ptr(vel));
	return vel;
}

glm::vec3 tr::audio_source::vel() const noexcept
{
	return _impl->vel();
}

void tr::_audio_source::set_vel(const glm::vec3& vel) noexcept
{
	TR_AL_CALL(alSourcefv, _id, AL_VELOCITY, value_ptr(vel));
}

void tr::audio_source::set_vel(const glm::vec3& vel) noexcept
{
	_impl->set_vel(vel);
}

void tr::audio_source::set_vel(const glm::vec3& vel, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::VEL, this->vel(), vel, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the velocity of an audio source.");
	}
}

//

glm::vec3 tr::_audio_source::dir() const noexcept
{
	glm::vec3 dir;
	TR_AL_CALL(alGetSourcefv, _id, AL_DIRECTION, value_ptr(dir));
	return dir;
}

glm::vec3 tr::audio_source::dir() const noexcept
{
	return _impl->dir();
}

void tr::_audio_source::set_dir(const glm::vec3& dir) noexcept
{
	TR_AL_CALL(alSourcefv, _id, AL_DIRECTION, value_ptr(dir));
}

void tr::audio_source::set_dir(const glm::vec3& dir) noexcept
{
	_impl->set_dir(dir);
}

void tr::audio_source::set_dir(const glm::vec3& dir, fsecs time) noexcept
{
	try {
		_impl->lock_audio_mutex();
		_audio_commands.emplace_back(_impl, _audio_command::type::DIR, this->dir(), dir, duration_cast<duration>(time));
		_impl->unlock_audio_mutex();
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the directory of an audio source.");
	}
}

//

tr::audio_origin tr::_audio_source::origin() const noexcept
{
	ALint origin;
	TR_AL_CALL(alGetSourcei, _id, AL_SOURCE_RELATIVE, &origin);
	return static_cast<audio_origin>(origin);
}

tr::audio_origin tr::audio_source::origin() const noexcept
{
	return _impl->origin();
}

void tr::_audio_source::set_origin(audio_origin type) noexcept
{
	TR_AL_CALL(alSourcei, _id, AL_SOURCE_RELATIVE, static_cast<ALint>(type));
}

void tr::audio_source::set_origin(audio_origin type) noexcept
{
	_impl->set_origin(type);
}

//

tr::audio_state tr::_audio_source::state() const noexcept
{
	ALint state;
	TR_AL_CALL(alGetSourcei, _id, AL_SOURCE_STATE, &state);
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

tr::audio_state tr::audio_source::state() const noexcept
{
	return _impl->state();
}

void tr::_audio_source::play() noexcept
{
	lock_audio_mutex();
	if (_stream.has_value()) {
		if (state() == tr::audio_state::INITIAL || state() == tr::audio_state::STOPPED) {
			TR_AL_CALL(alSourcei, _id, AL_BUFFER, 0);
			for (auto& buffer : _stream->buffers) {
				buffer.refill(*_stream);
				TR_AL_CALL(alSourceQueueBuffers, _id, 1, &buffer.id);
				if (_stream->stream->tell() == _stream->stream->length()) {
					break;
				}
			}
		}
	}
	TR_AL_CALL(alSourcePlay, _id);
	unlock_audio_mutex();
}

void tr::audio_source::play() noexcept
{
	_impl->play();
}

void tr::_audio_source::pause() noexcept
{
	TR_AL_CALL(alSourcePause, _id);
}

void tr::audio_source::pause() noexcept
{
	return _impl->pause();
}

void tr::_audio_source::stop() noexcept
{
	if (_stream.has_value()) {
		lock_audio_mutex();
		TR_AL_CALL(alSourceStop, _id);
		_stream->stream->seek(_stream->stream->loop_start());
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourceStop, _id);
	}
}

void tr::audio_source::stop() noexcept
{
	_impl->stop();
}

//

tr::fsecs tr::_audio_source::length() const noexcept
{
	if (_stream.has_value()) {
		return fsecs{static_cast<float>(_stream->stream->length()) / _stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate, size;
		TR_AL_CALL(alGetBufferi, _id, AL_FREQUENCY, &sample_rate);
		TR_AL_CALL(alGetBufferi, _id, AL_SIZE, &size);
		return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size) / sample_rate};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::length() const noexcept
{
	return _impl->length();
}

tr::fsecs tr::_audio_source::offset() const noexcept
{
	float offset;
	TR_AL_CALL(alGetSourcef, _id, AL_SEC_OFFSET, &offset);

	if (_stream.has_value()) {
		lock_audio_mutex();
		const tr::audio_state state{this->state()};
		if (state == tr::audio_state::INITIAL || state == tr::audio_state::STOPPED) {
			return fsecs{_stream->stream->tell() / static_cast<float>(_stream->stream->sample_rate())};
		}

		ALint buf_id;
		TR_AL_CALL(alGetSourcei, _id, AL_BUFFER, &buf_id);

		auto& buf{*std::ranges::find(_stream->buffers, static_cast<unsigned int>(buf_id), &_buffer_stream_buffer::id)};
		unlock_audio_mutex();
		return fsecs{buf.start_offset / static_cast<float>(_stream->stream->sample_rate()) + offset};
	}
	else {
		return fsecs{offset};
	}
}

tr::fsecs tr::audio_source::offset() const noexcept
{
	return _impl->offset();
}

void tr::_audio_source::set_offset(fsecs offset) noexcept
{
	if (_stream.has_value()) {
		lock_audio_mutex();
		audio_state state{this->state()};
		_stream->stream->seek(static_cast<int>(offset.count() * _stream->stream->sample_rate()));
		TR_AL_CALL(alSourceStop, _id);
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
		TR_AL_CALL(alSourcef, _id, AL_SEC_OFFSET, offset.count());
	}

	TR_AL_CALL(alSourcef, _id, AL_SEC_OFFSET, offset.count());
}

void tr::audio_source::set_offset(fsecs offset) noexcept
{
	_impl->set_offset(offset);
}

//

bool tr::_audio_source::looping() const noexcept
{
	if (_stream.has_value()) {
		return _stream->stream->looping();
	}
	else {
		ALint looping;
		TR_AL_CALL(alGetSourcei, _id, AL_LOOPING, &looping);
		return looping;
	}
}

bool tr::audio_source::looping() const noexcept
{
	return _impl->looping();
}

tr::fsecs tr::_audio_source::loop_start() const noexcept
{
	if (_stream.has_value()) {
		return fsecs{static_cast<float>(_stream->stream->loop_start()) / _stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, _id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[0]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::loop_start() const noexcept
{
	return _impl->loop_start();
}

tr::fsecs tr::_audio_source::loop_end() const noexcept
{
	if (_stream.has_value()) {
		return fsecs{static_cast<float>(_stream->stream->loop_end()) / _stream->stream->sample_rate()};
	}
	else if (buffer() != 0) {
		ALint sample_rate;
		TR_AL_CALL(alGetBufferi, buffer(), AL_FREQUENCY, &sample_rate);
		ALint channels;
		TR_AL_CALL(alGetBufferi, buffer(), AL_CHANNELS, &channels);

		std::array<ALint, 2> loop_points;
		TR_AL_CALL(alGetSourceiv, _id, AL_LOOP_POINTS_SOFT, loop_points.data());
		return fsecs{static_cast<float>(loop_points[1]) / sample_rate / channels};
	}
	else {
		return fsecs::zero();
	}
}

tr::fsecs tr::audio_source::loop_end() const noexcept
{
	return _impl->loop_end();
}

void tr::_audio_source::set_loop_points(fsecs start, fsecs end) noexcept
{
	if (length() == fsecs::zero()) {
		return;
	}

	start = std::clamp(start, START, length());
	end = std::clamp(end, START, length());
	TR_ASSERT(start < end, "Tried to set audio source loop end before start (start: {}s, end: {}s).", start.count(), end.count());

	if (_stream.has_value()) {
		lock_audio_mutex();
		if (start >= loop_end()) {
			_stream->stream->set_loop_end(static_cast<int>(end.count() * _stream->stream->sample_rate()));
			_stream->stream->set_loop_start(static_cast<int>(start.count() * _stream->stream->sample_rate()));
		}
		else {
			_stream->stream->set_loop_start(static_cast<int>(start.count() * _stream->stream->sample_rate()));
			_stream->stream->set_loop_end(static_cast<int>(end.count() * _stream->stream->sample_rate()));
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
		TR_AL_CALL(alSourcei, _id, AL_BUFFER, 0);
		TR_AL_CALL(alBufferiv, buffer, AL_LOOP_POINTS_SOFT, loop_points.data());
		TR_AL_CALL(alSourcei, _id, AL_BUFFER, buffer);
	}
}

void tr::audio_source::set_loop_points(fsecs start, fsecs end) noexcept
{
	_impl->set_loop_points(start, end);
}

void tr::_audio_source::set_looping(bool value) noexcept
{
	if (_stream.has_value()) {
		lock_audio_mutex();
		_stream->stream->set_looping(value);
		unlock_audio_mutex();
	}
	else {
		TR_AL_CALL(alSourcei, _id, AL_LOOPING, value);
	}
}

void tr::audio_source::set_looping(bool value) noexcept
{
	_impl->set_looping(value);
}