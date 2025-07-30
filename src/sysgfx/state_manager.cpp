#include "../../include/tr/sysgfx/state_manager.hpp"

std::uint32_t tr::state::type() const
{
	return NO_ID;
}

std::uint32_t tr::drop_state::type() const
{
	return std::numeric_limits<std::uint32_t>::max();
}

std::unique_ptr<tr::state> tr::drop_state::handle_event(const event&)
{
	return nullptr;
}

std::unique_ptr<tr::state> tr::drop_state::update(duration)
{
	return nullptr;
}

void tr::drop_state::draw() {}

tr::state_manager::state_manager(std::unique_ptr<tr::state>&& state)
	: state{std::move(state)}
{
}

const tr::benchmark& tr::state_manager::update_benchmark() const
{
	return update_bench;
}

const tr::benchmark& tr::state_manager::draw_benchmark() const
{
	return draw_bench;
}

void tr::state_manager::handle_event(const event& event)
{
	if (state != nullptr) {
		std::unique_ptr<tr::state> next{state->handle_event(event)};
		if (next != nullptr) {
			state = next->type() != std::numeric_limits<std::uint32_t>::max() ? std::move(next) : nullptr;
		}
	}
}

void tr::state_manager::update(duration delta)
{
	if (state != nullptr) {
		update_bench.start();
		std::unique_ptr<tr::state> next{state->update(delta)};
		update_bench.stop();
		if (next != nullptr) {
			state = next->type() != std::numeric_limits<std::uint32_t>::max() ? std::move(next) : nullptr;
			update_bench.clear();
			draw_bench.clear();
		}
	}
}

void tr::state_manager::draw()
{
	if (state != nullptr) {
		draw_bench.start();
		state->draw();
		draw_bench.stop();
	}
}