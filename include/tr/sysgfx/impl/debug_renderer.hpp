///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of debug_renderer.hpp.                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../debug_renderer.hpp"

///////////////////////////////////////////////////////////// DEBUG RENDERER //////////////////////////////////////////////////////////////

template <typename... Args> void tr::gfx::debug_renderer::write_left_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), default_style);
}

template <typename... Args>
void tr::gfx::debug_renderer::write_left_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), style);
}

template <typename... Args> void tr::gfx::debug_renderer::write_right_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), default_style);
}

template <typename... Args>
void tr::gfx::debug_renderer::write_right_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_right(std::format(fmt, std::forward<Args>(args)...), style);
}

template <tr::gfx::debug_writable_benchmark Benchmark, class R, class P>
void tr::gfx::debug_renderer::write_benchmark(const Benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit)
{
	write_benchmark(std::chrono::duration_cast<duration>(benchmark.min()), std::chrono::duration_cast<duration>(benchmark.avg()),
					std::chrono::duration_cast<duration>(benchmark.max()), name, std::chrono::duration_cast<duration>(limit));
}