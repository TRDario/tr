#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/audio/source.hpp"
#include <AL/alext.h>

///////////////////////////////////////////////////////////////// COMMAND /////////////////////////////////////////////////////////////////

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

tr::audio::command::argument tr::audio::command::value()
{
	const time_point now{clock::now()};
	m_elapsed = std::min(m_elapsed + now - m_last_update, m_length);
	m_last_update = now;
	const float t{duration_cast<fsecs>(m_elapsed) / m_length};

	argument value;
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
	argument v{value()};
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

///////////////////////////////////////////////////////////////// MANAGER /////////////////////////////////////////////////////////////////

bool tr::audio::manager::initialize()
{
	constexpr std::array<ALCint, 3> CONTEXT_ATTRIBUTES{ALC_HRTF_SOFT, ALC_FALSE, 0};

	std::unique_ptr<ALCdevice, device_closer> device{alcOpenDevice(nullptr)};
	if (device == nullptr) {
		return false;
	}

	std::unique_ptr<ALCcontext, context_destroyer> context{alcCreateContext(device.get(), CONTEXT_ATTRIBUTES.data())};
	if (context == nullptr || !alcMakeContextCurrent(context.get())) {
		return false;
	}

	int al_max_sources;
	alcGetIntegerv(device.get(), ALC_MONO_SOURCES, 1, &al_max_sources);
	m_max_sources = usize(al_max_sources);
	m_gains.fill(1);

	try {
		m_thread = std::jthread{&manager::thread_fn, this};
	}
	catch (...) {
		return false;
	}

	m_device = std::move(device);
	m_context = std::move(context);
	return true;
}

void tr::audio::manager::shut_down()
{
	m_thread.request_stop();
	m_thread.join();

	m_commands.clear();

	TR_ASSERT(std::ranges::all_of(m_sources, [](const std::shared_ptr<source_base>& ptr) { return ptr.use_count() == 1; }),
			  "Tried to shut down audio system while one or more audio sources still exists.");
	m_sources.clear();

	TR_ASSERT(std::ranges::all_of(m_buffers, [](const auto& pair) { return pair.second; }),
			  "Tried to shut down audio system while one or more audio buffers still exists.");
	m_buffers.clear();

	m_context.reset();
	m_device.reset();
}

void tr::audio::manager::device_closer::operator()(ALCdevice* device) const
{
	alcCloseDevice(device);
}

void tr::audio::manager::context_destroyer::operator()(ALCcontext* context) const
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
}

//

void tr::audio::manager::lock_mutex()
{
	m_mutex.lock();
}

void tr::audio::manager::unlock_mutex()
{
	m_mutex.unlock();
}

//

float tr::audio::manager::class_gain(int id) const
{
	return m_gains[id];
}

float tr::audio::manager::gain_multiplier(std::bitset<32> classes) const
{
	float multiplier{1.0f};
	for (int i = 0; i < 32; ++i) {
		if (classes[i]) {
			multiplier *= m_gains[i];
		}
	}
	return multiplier;
}

void tr::audio::manager::set_class_gain(int id, float gain)
{
	m_gains[id] = gain;
	for (source_base& source : deref(m_sources)) {
		if (source.classes()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

tr::audio::buffer::id tr::audio::manager::allocate_buffer()
{
	return m_buffers.emplace(buffer_base{}, false).first->first;
}

void tr::audio::manager::mark_buffer_as_cullable(buffer::id id)
{
	m_buffers.find(id)->second = true;
}

//

std::shared_ptr<tr::audio::source_base> tr::audio::manager::allocate_source(int priority)
{
	std::lock_guard lock{m_mutex};

	if (m_sources.size() == m_max_sources) {
		auto it{std::ranges::find_if(m_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
		if (it == m_sources.end()) {
			return nullptr;
		}
		else {
			m_sources.erase(it);
		}
	}

	return *m_sources.emplace(std::ranges::find_if(m_sources, [&](auto& s) { return s->priority() < priority; }),
							  std::make_shared<source_base>(priority));
}

//

void tr::audio::manager::thread_fn(std::stop_token stoken)
{
	TR_LOG(log, tr::severity::INFO, "Launched audio thread.");
	while (!stoken.stop_requested()) {
		try {
			std::lock_guard lock{m_mutex};

			std::erase_if(m_buffers, [this](auto& v) {
				auto& [buffer, cullable]{v};
				return cullable && std::ranges::none_of(m_sources, [&](auto& s) { return s->buffer() == buffer; });
			});

			m_sources.remove_if([](const auto& ptr) { return ptr.use_count() == 1 && ptr->state() != state::PLAYING; });
			for (source_base& source : deref(std::views::filter(m_sources, [](const auto& s) { return s->m_stream.has_value(); }))) {
				buffered_stream& stream{*source.m_stream};

				ALint nbuffers;
				TR_AL_CALL(alGetSourcei, source.m_id, AL_BUFFERS_PROCESSED, &nbuffers);
				if (nbuffers > 0) {
					std::array<buffer_base::id, 4> ids{};
					TR_AL_CALL(alSourceUnqueueBuffers, source.m_id, nbuffers, (ALuint*)ids.data());
					for (int i = 0; i < nbuffers; ++i) {
						if (!stream.stream->looping() && stream.stream->tell() == stream.stream->length()) {
							nbuffers = i;
							break;
						}
						std::ranges::find(stream.buffers, ids[i])->refill_from(*stream.stream);
					}
					if (nbuffers > 0) {
						TR_AL_CALL(alSourceQueueBuffers, source.m_id, nbuffers, (ALuint*)ids.data());
					}
				}
			}

			m_commands.remove_if([](command& c) {
				c.execute();
				return c.done();
			});
		}
		catch (std::exception& err) {
			TR_LOG(log, severity::ERROR, "Exception in audio thread, terminating.");
			TR_LOG_CONTINUE(log, "{}", err.what());
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{10});
	}
	TR_LOG(log, severity::INFO, "Returned from audio thread.");
}