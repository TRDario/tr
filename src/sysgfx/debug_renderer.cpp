#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/debug_renderer.hpp"
#include "../../include/tr/sysgfx/gpu_benchmark.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"

namespace tr::gfx {
#include "../../resources/generated/DEBUG_RENDERER_FONT.hpp"
#include "../../resources/generated/DEBUG_RENDERER_FRAG.hpp"
#include "../../resources/generated/DEBUG_RENDERER_VERT.hpp"

	// Debug renderer glyph vertex definition.
	template <> struct vertex_attributes<debug_renderer::glyph> : unpacked_vertex_attributes<glm::u8vec2, u8, u8, rgba8, rgba8> {};
	// Debug vertex format.
	constexpr std::array<vertex_binding, 2> DEBUG_FORMAT_ATTRIBUTES{{
		{NOT_INSTANCED, vertex_attributes<glm::u8vec2>::list},
		{1, unpacked_vertex_attributes<glm::u8vec2, u8, u8, rgba8, rgba8>::list},
	}};

	// Context passed to text formatting functions.
	struct debug_renderer::context {
		// The current line number.
		u8& line;
		// Whether the text is right-aligned.
		bool right_aligned;
		// The text color.
		rgba8 text_color;
		// The background color.
		rgba8 bg_color;
		// The length of the current line.
		u8 line_length;
		// The offset to the start of the text.
		usize text_start;
		// The offset to the start of the line.
		usize line_start;
		// The offset to the start of the word.
		usize word_start;
	};

	// Formats a duration into a string.
	std::string format_duration(std::string_view prefix, duration duration);
} // namespace tr::gfx

using namespace std::chrono_literals;

//

std::string tr::gfx::format_duration(std::string_view prefix, duration duration)
{
	if (duration <= 1us) {
		const double count{duration_cast<dnsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}ns", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else if (duration <= 1ms) {
		const double count{duration_cast<dusecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}us", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else if (duration <= 1s) {
		const double count{duration_cast<dmsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}ms", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else {
		const double count{duration_cast<dsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}s", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
}

//

tr::gfx::debug_renderer::debug_renderer(float scale, u8 column_limit)
	: m_pipeline{vertex_shader{DEBUG_RENDERER_VERT}, fragment_shader{DEBUG_RENDERER_FRAG}}
	, m_format{DEBUG_FORMAT_ATTRIBUTES}
	, m_font{load_embedded_bitmap(DEBUG_RENDERER_FONT)}
	, m_mesh{std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
	, m_column_limit{column_limit}
	, m_left_line{0}
	, m_right_line{0}
{
	TR_SET_LABEL(m_pipeline, "(tr) Debug Renderer Pipeline");
	TR_SET_LABEL(m_pipeline.vertex_shader(), "(tr) Debug Renderer Vertex Shader");
	TR_SET_LABEL(m_pipeline.fragment_shader(), "(tr) Debug Renderer Fragment Shader");
	TR_SET_LABEL(m_format, "(tr) Debug Renderer Vertex Format");
	TR_SET_LABEL(m_font, "(tr) Debug Renderer Font Texture");
	TR_SET_LABEL(m_mesh, "(tr) Debug Renderer Vertex Buffer");
	TR_SET_LABEL(m_glyph_buffer, "(tr) Debug Renderer Glyph buffer");

	m_font.set_filtering(min_filter::NEAREST, mag_filter::NEAREST);
	m_pipeline.fragment_shader().set_uniform(2, m_font);
	set_scale(scale);
}

void tr::gfx::debug_renderer::set_scale(float scale)
{
	m_pipeline.vertex_shader().set_uniform(1, scale);
}

void tr::gfx::debug_renderer::set_column_limit(u8 columns)
{
	m_column_limit = columns;
}

void tr::gfx::debug_renderer::write_left(std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	write(false, text, text_color, bg_color, extra_colors);
}

void tr::gfx::debug_renderer::write_right(std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	write(true, text, text_color, bg_color, extra_colors);
}

void tr::gfx::debug_renderer::write_right(const benchmark& benchmark, std::string_view name, duration limit)
{
	constexpr tr::rgba8 TEXT_COLOR{255, 255, 255, 255};
	constexpr tr::rgba8 ALT_COLOR{255, 0, 0, 255};

	if (!name.empty()) {
		write_right(TR_FMT::format("{:<15}", name));
	}
	write_right(format_duration("MIN: ", benchmark.min()), benchmark.min() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("AVG: ", benchmark.avg()), benchmark.avg() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("MAX: ", benchmark.max()), benchmark.max() < limit ? TEXT_COLOR : ALT_COLOR);
}

void tr::gfx::debug_renderer::write_right(const gpu_benchmark& benchmark, std::string_view name, duration limit)
{
	constexpr tr::rgba8 TEXT_COLOR{255, 255, 255, 255};
	constexpr tr::rgba8 ALT_COLOR{255, 0, 0, 255};

	if (!name.empty()) {
		write_right(TR_FMT::format("{:<15}", name));
	}
	write_right(format_duration("MIN: ", benchmark.min()), benchmark.min() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("AVG: ", benchmark.avg()), benchmark.avg() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("MAX: ", benchmark.max()), benchmark.max() < limit ? TEXT_COLOR : ALT_COLOR);
}

void tr::gfx::debug_renderer::newline_left()
{
	++m_left_line;
}

void tr::gfx::debug_renderer::newline_right()
{
	++m_right_line;
}

void tr::gfx::debug_renderer::draw()
{
	if (!m_glyphs.empty()) {
		m_glyph_buffer.set(m_glyphs);
		m_pipeline.vertex_shader().set_uniform(0, glm::vec2{backbuffer_size()});

		set_render_target(backbuffer_render_target());
		if (should_setup_context(renderer_id::DEBUG_RENDERER)) {
			set_face_culling(false);
			set_depth_test(false);
			set_blend_mode(ALPHA_BLENDING);
			set_shader_pipeline(m_pipeline);
			set_vertex_format(m_format);
			set_vertex_buffer(m_mesh, 0, 0);
			set_vertex_buffer(m_glyph_buffer, 1, 0);
		}
		draw_instances(primitive::TRI_FAN, 0, 4, int(m_glyphs.size()));

		m_glyphs.clear();
	}

	m_left_line = 0;
	m_right_line = 0;
}

//

void tr::gfx::debug_renderer::right_align_line(usize begin, usize end)
{
	for (usize i = begin; i < end; ++i) {
		m_glyphs[i].pos.x = u8(end - i);
	}
}

void tr::gfx::debug_renderer::trim_trailing_whitespace(context& context)
{
	usize line_end = context.word_start - 1;
	while (line_end > context.line_start && m_glyphs[line_end].chr != ' ') {
		--line_end;
	}

	if (line_end != context.word_start) {
		m_glyphs.erase(m_glyphs.begin() + line_end, m_glyphs.begin() + context.word_start);
		context.word_start = line_end;
	}
}

void tr::gfx::debug_renderer::move_word_to_next_line(context& context)
{
	for (glyph& glyph : std::ranges::subrange{m_glyphs.begin() + context.word_start, m_glyphs.end()}) {
		glyph.pos = {context.line_length++, context.line};
	}
	context.line_start = context.word_start;
}

void tr::gfx::debug_renderer::break_at_last_whitespace(context& context)
{
	trim_trailing_whitespace(context);
	if (context.right_aligned) {
		right_align_line(context.line_start, context.word_start);
	}
	move_word_to_next_line(context);
}

void tr::gfx::debug_renderer::break_overlong_word(context& context)
{
	if (context.right_aligned) {
		right_align_line(context.line_start, context.line_start + m_column_limit);
	}
	m_glyphs.back().pos = {context.line_length++, context.line};

	context.line_start += m_column_limit;
	context.word_start = context.line_start;
}

void tr::gfx::debug_renderer::handle_column_limit(context& context)
{
	context.line_length = 0;
	++context.line;

	if (context.word_start > context.line_start) {
		break_at_last_whitespace(context);
	}
	else {
		break_overlong_word(context);
	}
}

void tr::gfx::debug_renderer::write_character(char chr, context& context)
{
	if (context.line_length == m_column_limit && chr == ' ') {
		handle_newline(context);
	}
	else {
		m_glyphs.push_back({{context.line_length, context.line}, context.right_aligned, chr, context.text_color, context.bg_color});

		if (m_glyphs.size() - context.text_start > 1) {
			const char prev{m_glyphs[m_glyphs.size() - 2].chr};
			if (prev == ' ' && chr != ' ') {
				context.word_start = m_glyphs.size() - 1;
			}
		}

		if (++context.line_length > m_column_limit) {
			handle_column_limit(context);
		}
	}
}

void tr::gfx::debug_renderer::handle_newline(context& context)
{
	if (context.right_aligned) {
		right_align_line(context.line_start, m_glyphs.size());
	}

	context.line_length = 0;
	context.word_start = m_glyphs.size();
	context.line_start = m_glyphs.size();
	++context.line;
}

void tr::gfx::debug_renderer::handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, context& context,
													  rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	switch (*it) {
	case 'b':
		if (std::next(it) != end && std::isdigit(*++it) && u8(*it - '0') < extra_colors.size()) {
			context.bg_color = extra_colors[*it - '0'];
		}
		break;
	case 'B':
		context.bg_color = bg_color;
		break;
	case 'c':
		if (std::next(it) != end && std::isdigit(*++it) && u8(*it - '0') < extra_colors.size()) {
			context.text_color = extra_colors[*it - '0'];
		}
		break;
	case 'C':
		context.text_color = text_color;
		break;
	case 'n':
		handle_newline(context);
		break;
	case '$':
		write_character('$', context);
		break;
	default:
		break;
	}
}

void tr::gfx::debug_renderer::write(bool right, std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	const usize old_size{m_glyphs.size()};
	context context{right ? m_right_line : m_left_line, right, text_color, bg_color, 0, old_size, old_size, old_size};

	for (std::string_view::iterator it = text.begin(); it != text.end(); ++it) {
		if (*it == '$') {
			if (std::next(it) != text.end()) {
				handle_control_sequence(++it, text.end(), context, text_color, bg_color, extra_colors);
			}
		}
		else {
			write_character(*it, context);
		}
	}
	handle_newline(context);
}