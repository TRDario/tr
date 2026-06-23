///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements audio_source.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/audio/audio_context.hpp"
#include "../../include/tr/audio/audio_stream.hpp"
#include "../../include/tr/utility/ranges.hpp"
#include "../../include/tr/utility/variant.hpp"
#include <AL/al.h>
#include <AL/alext.h>

////////////////////////////////////////////////////////// BUFFERED STREAM BUFFER /////////////////////////////////////////////////////////

tr::audio_source::buffered_stream::buffer::buffer(audio_context& context)
	: audio_buffer{context}
{
}

//

tr::usize tr::audio_source::buffered_stream::buffer::start_offset() const
{
	return m_start_offset;
}

void tr::audio_source::buffered_stream::buffer::refill_from(audio_stream& source)
{
	m_start_offset = source.tell();
	std::array<i16, 16384> data;
	std::span<const i16> used_data{source.read(data)};
	used_data = used_data.subspan(0, used_data.size() - used_data.size() % 2);
	set(used_data, source.channels() == 2 ? audio_format::stereo16 : audio_format::mono16, source.sample_rate());
}

///////////////////////////////////////////////////////////// BUFFERED STREAM /////////////////////////////////////////////////////////////

tr::audio_source::buffered_stream::buffered_stream(audio_context& context, std::unique_ptr<audio_stream>&& stream)
	: m_stream{std::move(stream)}
	, m_buffers{context, context, context, context}
{
}

//

tr::fsecs tr::audio_source::buffered_stream::length() const
{
	return fsecs{float(m_stream->length()) / m_stream->sample_rate()};
}

//

tr::fsecs tr::audio_source::buffered_stream::tell() const
{
	return fsecs{float(m_stream->tell()) / m_stream->sample_rate()};
}

void tr::audio_source::buffered_stream::seek(fsecs where)
{
	m_stream->seek(where.count() * m_stream->sample_rate());
}

//

bool tr::audio_source::buffered_stream::looping() const
{
	return m_stream->looping();
}

tr::fsecs tr::audio_source::buffered_stream::loop_start() const
{
	return fsecs{float(m_stream->loop_start()) / m_stream->sample_rate()};
}

tr::fsecs tr::audio_source::buffered_stream::loop_end() const
{
	return fsecs{float(m_stream->loop_end()) / m_stream->sample_rate()};
}

void tr::audio_source::buffered_stream::set_looping(bool looping)
{
	m_stream->set_looping(looping);
}

void tr::audio_source::buffered_stream::set_loop_points(fsecs start_point, fsecs end_point)
{
	if (start_point >= loop_end()) {
		m_stream->set_loop_end(int(end_point.count() * m_stream->sample_rate()));
		m_stream->set_loop_start(int(start_point.count() * m_stream->sample_rate()));
	}
	else {
		m_stream->set_loop_start(int(start_point.count() * m_stream->sample_rate()));
		m_stream->set_loop_end(int(end_point.count() * m_stream->sample_rate()));
	}
}

//

tr::fsecs tr::audio_source::buffered_stream::buffer_start_offset(unsigned int id) const
{
	const usize raw_offset{std::ranges::find(m_buffers, id, [](const buffer& buffer) { return buffer.m_handle.get(); })->start_offset()};
	return fsecs{float(raw_offset) / m_stream->sample_rate()};
}

//

tr::static_vector<unsigned int, 4> tr::audio_source::buffered_stream::try_refill_all()
{
	return try_refill(m_buffers | std::views::transform([](buffer& b) { return b.m_handle.get(); }));
}

tr::static_vector<unsigned int, 4> tr::audio_source::buffered_stream::try_refill(static_vector<unsigned int, 4> buffers)
{
	for (int i = 0; i < buffers.size(); ++i) {
		if (!m_stream->looping() && m_stream->tell() == m_stream->length()) {
			buffers.resize(i);
			break;
		}
		std::ranges::find(m_buffers, buffers[i], [](buffer& b) { return b.m_handle.get(); })->refill_from(*m_stream);
	}
	return buffers;
}

/////////////////////////////////////////////////////////////// AUDIO SOURCE //////////////////////////////////////////////////////////////

tr::audio_source::audio_source(audio_context& context, int priority)
	: m_handle{{context}}
	, m_priority{priority}
	, m_class_mask{}
	, m_gain{1.0f}
{
	context.m_alapi.generate_sources(context.m_ptr.get(), 1, out_handle(m_handle));
}

void tr::audio_source::deleter::operator()(unsigned int id) const
{
	context.m_alapi.delete_sources(context.m_ptr.get(), 1, &id);
}

//

std::shared_ptr<tr::audio_source> tr::create_audio_source(audio_context& context, int priority)
{
	const std::lock_guard lock{context.m_mutex};
	if (context.m_sources.size() == context.m_max_sources) {
		auto erasable_it{std::ranges::find_if(context.m_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
		if (erasable_it == context.m_sources.end()) {
			return nullptr;
		}
		else {
			unstable_erase(context.m_sources, erasable_it);
		}
	}
	return context.m_sources.emplace_back(new audio_source{context, priority});
}

//

tr::audio_context& tr::audio_source::context() const
{
	return m_handle.get_deleter().context;
}

//

void tr::audio_source::use(std::shared_ptr<audio_buffer> buffer)
{
	use(std::lock_guard{context().m_mutex}, buffer);
}

void tr::audio_source::use(const std::lock_guard<std::mutex>& lock, std::shared_ptr<audio_buffer> buffer)
{
	audio_context& ctx{context()};

	clear(lock);
	attach_buffer(*buffer);
	ctx.m_alapi.set_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_DIRECT_CHANNELS_SOFT, buffer->channels() == 2);
	m_data_source.emplace<std::shared_ptr<audio_buffer>>(std::move(buffer));
	set_loop_points(lock, start, end);
}

void tr::audio_source::use(std::unique_ptr<audio_stream>&& stream)
{
	use(std::lock_guard{context().m_mutex}, std::move(stream));
}

void tr::audio_source::use(const std::lock_guard<std::mutex>& lock, std::unique_ptr<audio_stream>&& stream)
{
	clear(lock);
	m_data_source.emplace<buffered_stream>(context(), std::move(stream));
}

void tr::audio_source::clear()
{
	clear(std::lock_guard{context().m_mutex});
}

void tr::audio_source::clear(const std::lock_guard<std::mutex>& lock)
{
	stop(lock);
	set_loop_points(lock, start, end);
	detach_buffer();
	m_data_source.emplace<std::monostate>();
	set_looping(lock, false);
}

//

int tr::audio_source::priority() const
{
	return m_priority;
}

const tr::audio_class_mask& tr::audio_source::class_mask() const
{
	return m_class_mask;
}

void tr::audio_source::set_class_mask(const audio_class_mask& mask)
{
	m_class_mask = mask;
	set_gain(m_gain);
}

//

float tr::audio_source::pitch() const
{
	float pitch;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_PITCH, &pitch);
	return pitch;
}

void tr::audio_source::set_pitch(float pitch)
{
	TR_ASSERT(pitch > 0.0f, "Tried to set audio source pitch to invalid value '{}'.", pitch);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_PITCH, pitch);
}

void tr::audio_source::set_pitch(float end_pitch, fsecs time)
{
	context().create_command(*this, &audio_source::set_pitch, pitch(), end_pitch, time);
}

//

float tr::audio_source::gain() const
{
	return m_gain;
}

void tr::audio_source::set_gain(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set audio source gain to invalid value '{}'.", gain);

	m_gain = gain;
	for (usize i = 0; i < audio_class_count; ++i) {
		if (m_class_mask[i]) {
			gain *= context().m_class_gains[i];
		}
	}

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_GAIN, gain);
}

void tr::audio_source::set_gain(float end_gain, fsecs time)
{
	context().create_command(*this, &audio_source::set_gain, gain(), end_gain, time);
}

//

float tr::audio_source::max_distance() const
{
	float distance;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_MAX_DISTANCE, &distance);
	return distance;
}

void tr::audio_source::set_max_distance(float distance)
{
	TR_ASSERT(distance >= 0.0f, "Tried to set audio source max distance to invalid value '{}'.", distance);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_MAX_DISTANCE, distance);
}

void tr::audio_source::set_max_distance(float end_distance, fsecs time)
{
	context().create_command(*this, &audio_source::set_max_distance, max_distance(), end_distance, time);
}

//

float tr::audio_source::rolloff_factor() const
{
	float rolloff;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_ROLLOFF_FACTOR, &rolloff);
	return rolloff;
}

void tr::audio_source::set_rolloff_factor(float rolloff)
{
	TR_ASSERT(rolloff >= 0.0f, "Tried to set audio source rolloff factor to invalid value '{}'.", rolloff);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_ROLLOFF_FACTOR, rolloff);
}

void tr::audio_source::set_rolloff_factor(float end_rolloff, fsecs time)
{
	context().create_command(*this, &audio_source::set_rolloff_factor, rolloff_factor(), end_rolloff, time);
}

//

float tr::audio_source::reference_distance() const
{
	float ref_dist;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_REFERENCE_DISTANCE, &ref_dist);
	return ref_dist;
}

void tr::audio_source::set_reference_distance(float distance)
{
	TR_ASSERT(distance >= 0.0f, "Tried to set audio source reference distance to invalid value '{}'.", distance);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_REFERENCE_DISTANCE, distance);
}

void tr::audio_source::set_reference_distance(float end_distance, fsecs time)
{
	context().create_command(*this, &audio_source::set_reference_distance, reference_distance(), end_distance, time);
}

//

float tr::audio_source::gain_outside_cone() const
{
	float out_gain;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_OUTER_GAIN, &out_gain);
	return out_gain;
}

void tr::audio_source::set_gain_outside_cone(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set audio source gain outside cone to invalid value '{}'.", gain);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_OUTER_GAIN, gain);
}

void tr::audio_source::set_gain_outside_cone(float end_gain, fsecs time)
{
	context().create_command(*this, &audio_source::set_gain_outside_cone, gain_outside_cone(), end_gain, time);
}

//

tr::angle tr::audio_source::inner_cone_width() const
{
	float width;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_INNER_ANGLE, &width);
	return degs(width);
}

tr::angle tr::audio_source::outer_cone_width() const
{
	float width;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_OUTER_ANGLE, &width);
	return degs(width);
}

void tr::audio_source::set_cone_widths(angle inner, angle outer)
{
	TR_ASSERT(inner >= 0_deg && inner < 360_deg, "Tried to set audio source inner cone width to invalid value '{:d}'.", inner);
	TR_ASSERT(outer >= 0_deg && outer < 360_deg, "Tried to set audio source outer cone width to invalid value '{:d}'.", outer);
	TR_ASSERT(inner < outer, "Tried to set audio source outer cone as thinner than inner cone (inner: {:d}, outer: {:d}).", inner, outer);

	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_INNER_ANGLE, inner.degs());
	ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_CONE_OUTER_ANGLE, outer.degs());
}

void tr::audio_source::set_cone_widths(angle inner, angle outer, fsecs time)
{
	context().create_command(*this, &audio_source::set_cone_widths, std::tuple{inner_cone_width(), outer_cone_width()},
							 std::tuple{inner, outer}, time);
}

//

glm::vec3 tr::audio_source::position() const
{
	glm::vec3 pos;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio_source::set_position(const glm::vec3& pos)
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_POSITION, value_ptr(pos));
}

void tr::audio_source::set_position(const glm::vec3& end_position, fsecs time)
{
	context().create_command(*this, &audio_source::set_position, position(), end_position, time);
}

//

glm::vec3 tr::audio_source::velocity() const
{
	glm::vec3 vel;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio_source::set_velocity(const glm::vec3& vel)
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_VELOCITY, value_ptr(vel));
}

void tr::audio_source::set_velocity(const glm::vec3& end_velocity, fsecs time)
{
	context().create_command(*this, &audio_source::set_velocity, velocity(), end_velocity, time);
}

//

glm::vec3 tr::audio_source::direction() const
{
	glm::vec3 dir;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_DIRECTION, value_ptr(dir));
	return dir;
}

void tr::audio_source::set_direction(const glm::vec3& dir)
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_fv(ctx.m_ptr.get(), m_handle.get(), AL_DIRECTION, value_ptr(dir));
}

void tr::audio_source::set_direction(const glm::vec3& end_direction, fsecs time)
{
	context().create_command(*this, &audio_source::set_direction, direction(), end_direction, time);
}

//

enum tr::audio_source::origin tr::audio_source::origin() const
{
	ALint origin;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_SOURCE_RELATIVE, &origin);
	return (enum origin)(origin);
}

void tr::audio_source::set_origin(enum origin type)
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_SOURCE_RELATIVE, ALint(type));
}

//

enum tr::audio_source::state tr::audio_source::state() const
{
	ALint state;
	audio_context& ctx{context()};
	ctx.m_alapi.get_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_SOURCE_STATE, &state);
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

void tr::audio_source::play()
{
	play(std::lock_guard{context().m_mutex});
}

void tr::audio_source::play(const std::lock_guard<std::mutex>&)
{
	audio_context& ctx{context()};

	const tr::opt_ref<buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		if (state() == state::initial || state() == state::stopped) {
			detach_buffer();
			const static_vector<unsigned int, 4> filled_buffers{stream->try_refill_all()};
			if (!filled_buffers.empty()) {
				ctx.m_alapi.source_queue_buffers(ctx.m_ptr.get(), m_handle.get(), filled_buffers.size(), filled_buffers.data());
			}
		}
	}

	ctx.m_alapi.source_play(ctx.m_ptr.get(), m_handle.get());
}

void tr::audio_source::pause()
{
	audio_context& ctx{context()};
	ctx.m_alapi.source_pause(ctx.m_ptr.get(), m_handle.get());
}

void tr::audio_source::stop()
{
	stop(std::lock_guard{context().m_mutex});
}

void tr::audio_source::stop(const std::lock_guard<std::mutex>&)
{
	audio_context& ctx{context()};
	ctx.m_alapi.source_stop(ctx.m_ptr.get(), m_handle.get());

	const tr::opt_ref<buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		stream->seek(stream->loop_start());
	}
}

//

tr::fsecs tr::audio_source::length() const
{
	// clang-format off
	return m_data_source | tr::match{
		[](const buffered_stream& stream) {
			return stream.length();
		},
		[](const std::shared_ptr<audio_buffer>& buffer) {
			const int sample_rate{buffer->sample_rate()};
			return sample_rate == 0 ? fsecs::zero() : fsecs{double(buffer->size()) / sample_rate};
		},
		[](std::monostate) {
			return fsecs::zero();
		}
	};
	// clang-format on
}

tr::fsecs tr::audio_source::offset() const
{
	audio_context& ctx{context()};

	float offset;
	ctx.m_alapi.get_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_SEC_OFFSET, &offset);

	const tr::opt_ref<const buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		const auto cur_state{this->state()};
		if (cur_state == state::initial || cur_state == state::stopped) {
			return stream->tell();
		}

		int current_buffer_id;
		ctx.m_alapi.get_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_BUFFER, &current_buffer_id);
		return stream->buffer_start_offset(current_buffer_id) + fsecs{offset};
	}
	else {
		return fsecs{offset};
	}
}

void tr::audio_source::set_offset(fsecs offset)
{
	set_offset(std::lock_guard{context().m_mutex}, offset);
}

void tr::audio_source::set_offset(const std::lock_guard<std::mutex>& lock, fsecs offset)
{
	audio_context& ctx{context()};

	const tr::opt_ref<buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		const auto prev_state{state()};
		stream->seek(offset);
		ctx.m_alapi.source_stop(ctx.m_ptr.get(), m_handle.get());
		switch (prev_state) {
		case state::playing:
			play(lock);
			break;
		case state::paused:
			play(lock);
			pause();
			break;
		case state::initial:
		case state::stopped:
			break;
		}
	}
	else {
		ctx.m_alapi.set_source_property_f(ctx.m_ptr.get(), m_handle.get(), AL_SEC_OFFSET, offset.count());
	}
}

//

bool tr::audio_source::looping() const
{
	const tr::opt_ref<const buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		return stream->looping();
	}
	else {
		audio_context& ctx{context()};
		ALint looping;
		ctx.m_alapi.get_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_LOOPING, &looping);
		return looping;
	}
}

tr::fsecs tr::audio_source::loop_start() const
{
	return m_data_source | tr::match{
							   [](const buffered_stream& stream) { return stream.loop_start(); },
							   [](const std::shared_ptr<audio_buffer>& buffer) { return buffer->loop_points().first; },
							   [](std::monostate) { return fsecs::zero(); },
						   };
}

tr::fsecs tr::audio_source::loop_end() const
{
	return m_data_source | tr::match{
							   [](const buffered_stream& stream) { return stream.loop_end(); },
							   [](const std::shared_ptr<audio_buffer>& buffer) { return buffer->loop_points().second; },
							   [](std::monostate) { return fsecs::zero(); },
						   };
}

void tr::audio_source::set_looping(bool value)
{
	set_looping(std::lock_guard{context().m_mutex}, value);
}

void tr::audio_source::set_looping(const std::lock_guard<std::mutex>&, bool value)
{
	const tr::opt_ref<buffered_stream> stream{tr::get_if<buffered_stream>(m_data_source)};
	if (stream.has_ref()) {
		stream->set_looping(value);
	}
	else {
		audio_context& ctx{context()};
		ctx.m_alapi.set_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_LOOPING, value);
	}
}

void tr::audio_source::set_loop_points(fsecs start_point, fsecs end_point)
{
	set_loop_points(std::lock_guard{context().m_mutex}, start_point, end_point);
}

void tr::audio_source::set_loop_points(const std::lock_guard<std::mutex>&, fsecs start_point, fsecs end_point)
{
	if (length() == fsecs::zero()) {
		return;
	}

	start_point = std::clamp(start_point, start_point, length());
	end_point = std::clamp(end_point, start_point, length());
	TR_ASSERT(start_point < end_point, "Tried to set audio source loop end before start (start: {}s, end: {}s).", start_point.count(),
			  end_point.count());

	struct visitor {
		audio_source& source;
		fsecs start_point;
		fsecs end_point;

		void operator()(buffered_stream& stream) const
		{
			stream.set_loop_points(start_point, end_point);
		}
		void operator()(std::shared_ptr<audio_buffer>& buffer) const
		{
			source.detach_buffer();
			buffer->set_loop_points(start_point, end_point);
			source.attach_buffer(*buffer);
		}
		void operator()(std::monostate) const {}
	};
	std::visit(visitor{*this, start_point, end_point}, m_data_source);
}

//

void tr::audio_source::attach_buffer(audio_buffer& buffer)
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_BUFFER, buffer.m_handle.get());
}

void tr::audio_source::detach_buffer()
{
	audio_context& ctx{context()};
	ctx.m_alapi.set_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_BUFFER, 0);
}

//

void tr::audio_source::refill_if_needed()
{
	tr::opt_ref<buffered_stream> bstream{get_if<buffered_stream>(m_data_source)};
	if (bstream.has_ref()) {
		audio_context& ctx{context()};

		ALint nbuffers;
		ctx.m_alapi.get_source_property_i(ctx.m_ptr.get(), m_handle.get(), AL_BUFFERS_PROCESSED, &nbuffers);
		if (nbuffers == 0) {
			return;
		}

		static_vector<unsigned int, 4> buffers(nbuffers);
		ctx.m_alapi.source_unqueue_buffers(ctx.m_ptr.get(), m_handle.get(), nbuffers, buffers.data());
		const static_vector<unsigned int, 4> filled_buffers{bstream->try_refill(buffers)};
		if (!filled_buffers.empty()) {
			ctx.m_alapi.source_queue_buffers(ctx.m_ptr.get(), m_handle.get(), filled_buffers.size(), filled_buffers.data());
		}
	}
}