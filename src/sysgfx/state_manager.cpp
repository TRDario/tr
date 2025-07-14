#include "../../include/tr/sysgfx/state_manager.hpp"

std::uint32_t tr::state::type() const noexcept
{
	return NO_ID;
}

std::uint32_t tr::drop_state::type() const noexcept
{
	return std::numeric_limits<std::uint32_t>::max();
}

std::unique_ptr<tr::state> tr::drop_state::handle_event(const event&) noexcept
{
	return nullptr;
}

std::unique_ptr<tr::state> tr::drop_state::update(duration) noexcept
{
	return nullptr;
}

void tr::drop_state::draw() noexcept {}

tr::state_manager::state_manager(std::unique_ptr<tr::state>&& state) noexcept
	: state{std::move(state)}
{
}

const tr::benchmark& tr::state_manager::update_benchmark() const noexcept
{
	return _update_benchmark;
}

const tr::benchmark& tr::state_manager::draw_benchmark() const noexcept
{
	return _draw_benchmark;
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
		_update_benchmark.start();
		std::unique_ptr<tr::state> next{state->update(delta)};
		_update_benchmark.stop();
		if (next != nullptr) {
			state = next->type() != std::numeric_limits<std::uint32_t>::max() ? std::move(next) : nullptr;
			_update_benchmark.clear();
			_draw_benchmark.clear();
		}
	}
}

void tr::state_manager::draw()
{
	if (state != nullptr) {
		_draw_benchmark.start();
		state->draw();
		_draw_benchmark.stop();
	}
}