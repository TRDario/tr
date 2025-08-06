#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/debug_renderer.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/utility/benchmark.hpp"

namespace tr::gfx::debug_renderer {
#include "resources/debug_font.bmp.hpp"

	// Debug vertex shader.
	inline constexpr const char* DEBUG_RENDERER_VERT_SRC =
		"#version 450\n#define L(l) layout(location=l)\nlayout(std430,binding=0)buffer G{int g[];};L(0)uniform vec2 "
		"r;L(1)uniform float s;L(0)in vec2 p;out gl_PerVertex{vec4 gl_Position;};L(0)out vec2 o;L(1)out vec4 T;L(2)out vec4 B;void "
		"main(){const int m=255,i=gl_InstanceID*3,a=g[i],t=g[i+1],b=g[i+2],h=a>>24&m;o=(vec2(h%16,h/16)+p)/"
		"16;T=vec4(t&m,t>>8&m,t>>16&m,t>>24&m)/m;B=vec4(b&m,b>>8&m,b>>16&m,b>>24&m)/m;vec2 "
		"C=vec2(a&m,a>>8&m),P;if((a>>16&m)!=0){P=C*8*s;P.x=r.x-P.x;P+=p*8*s;}else{P=(p+C)*8*s;}gl_Position=vec4(P.x/"
		"r.x*2-1,-P.y/r.y*2+1,0,1);}";
	// Debug fragment shader.
	inline constexpr const char* DEBUG_RENDERER_FRAG_SRC =
		"#version 450\n#define L(l) layout(location=l)\nL(2)uniform sampler2D t;L(0)in vec2 u;L(1)in vec4 c;L(2)in "
		"vec4 b;L(0)out vec4 o;void main(){o=texture(t,u).r==1?c:b;}";
	// Debug renderer ID.
	inline constexpr std::uint32_t DEBUG_RENDERER{2};

	// Debug renderer glyph.
	struct glyph {
		// Position on the glyph grid.
		glm::u8vec2 pos;
		// Whether the glyph should be right-aligned.
		bool align_right;
		// The ASCII value of the glyph.
		char chr;
		// The color of the text.
		rgba8 text_color;
		// The color of the background.
		rgba8 bg_color;
	};

	// Debug renderer implementation.
	struct state_t {
		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline pipeline;
		// The debug vertex format.
		vertex_format format;
		// The shader buffer holding the glyphs.
		shader_buffer sbuffer;
		// The font texture.
		texture font;
		// The texture unit used by the renderer.
		texture_unit tex_unit;
		// The vertex buffer holding a glyph shape.
		static_vertex_buffer<glm::u8vec2> vbuffer;
		// List of glyphs to draw.
		std::vector<glyph> glyphs;
		// Maximum allowed number of glyphs per line.
		std::uint8_t column_limit;
		// The current left line position.
		std::uint8_t left_line;
		// The current right line position.
		std::uint8_t right_line;

		// Creates the debug renderer.
		state_t();
	};
	// Debug renderer state.
	std::optional<state_t> state;

	// Context passed to text formatting functions.
	struct context {
		// The current line number.
		std::uint8_t& line;
		// Whether the text is right-aligned.
		bool right_aligned;
		// The text color.
		rgba8 text_color;
		// The background color.
		rgba8 bg_color;
		// The length of the current line.
		std::uint8_t line_length;
		// The offset to the start of the text.
		std::size_t text_start;
		// The offset to the start of the line.
		std::size_t line_start;
		// The offset to the start of the word.
		std::size_t word_start;
	};

	// Formats a duration into a string.
	std::string format_duration(std::string_view prefix, duration duration);
	// Right-aligns a line.
	void right_align_line(std::size_t begin, std::size_t end);
	// Trims trailing whitespace in a line.
	void trim_trailing_whitespace(context& context);
	// Moves a word to the next line.
	void move_word_to_next_line(context& context);
	// Breaks a line at the last whitespace.
	void break_at_last_whitespace(context& context);
	// Breaks an overlong word.
	void break_overlong_word(context& context);
	// Handles a line reaching the column limit.
	void handle_column_limit(context& context);
	// Writes a character.
	void write_character(char chr, context& context);
	// Handles a newline.
	void handle_newline(context& context);
	// Handles a control sequence.
	void handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, context& context, rgba8 text_color,
								 rgba8 bg_color, std::span<rgba8> extra_colors);
	// Writes a line of formatted text.
	void write(bool right, std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors);
} // namespace tr::gfx::debug_renderer

using namespace std::chrono_literals;

tr::gfx::debug_renderer::state_t::state_t()
	: pipeline{vertex_shader{DEBUG_RENDERER_VERT_SRC}, fragment_shader{DEBUG_RENDERER_FRAG_SRC}}
	, format{{NOT_INSTANCED, {vertex_attributef{vertex_attributef::type::UI8, 2, false, 0}}}}
	, sbuffer{0, 256 * sizeof(glyph), shader_buffer::access::WRITE_ONLY}
	, font{load_embedded_bitmap(DEBUG_RENDERER_FONT)}
	, vbuffer{std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
	, column_limit{255}
	, left_line{0}
	, right_line{0}
{
	font.set_filtering(min_filter::NEAREST, mag_filter::NEAREST);
	tex_unit.set_texture(font);
	pipeline.fragment_shader().set_uniform(2, tex_unit);
	if (debug()) {
		pipeline.set_label("(tr) Debug Renderer Pipeline");
		pipeline.vertex_shader().set_label("(tr) Debug Renderer Vertex Shader");
		pipeline.fragment_shader().set_label("(tr) Debug Renderer Fragment Shader");
		format.set_label("(tr) Debug Renderer Vertex Format");
		sbuffer.set_label("(tr) Debug Renderer Shader Glyph Buffer");
		font.set_label("(tr) Debug Renderer Font Texture");
		vbuffer.set_label("(tr) Debug Renderer Vertex Buffer");
	}
}

std::string tr::gfx::debug_renderer::format_duration(std::string_view prefix, duration duration)
{
	if (duration <= 1us) {
		const double count{duration_cast<dnsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}ns", prefix, precision), std::make_format_args(count));
	}
	else if (duration <= 1ms) {
		const double count{duration_cast<dusecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}us", prefix, precision), std::make_format_args(count));
	}
	else if (duration <= 1s) {
		const double count{duration_cast<dmsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}ms", prefix, precision), std::make_format_args(count));
	}
	else {
		const double count{duration_cast<dsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}s ", prefix, precision), std::make_format_args(count));
	}
}

void tr::gfx::debug_renderer::right_align_line(std::size_t begin, std::size_t end)
{
	for (std::size_t i = begin; i < end; ++i) {
		state->glyphs[i].pos.x = static_cast<std::uint8_t>(end - i);
	}
}

void tr::gfx::debug_renderer::trim_trailing_whitespace(context& context)
{
	std::size_t line_end = context.word_start - 1;
	while (line_end > context.line_start && state->glyphs[line_end].chr != ' ') {
		--line_end;
	}

	if (line_end != context.word_start) {
		state->glyphs.erase(state->glyphs.begin() + line_end, state->glyphs.begin() + context.word_start);
		context.word_start = line_end;
	}
}

void tr::gfx::debug_renderer::move_word_to_next_line(context& context)
{
	for (auto it = state->glyphs.begin() + context.word_start; it != state->glyphs.end(); ++it) {
		it->pos = {context.line_length++, context.line};
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
		right_align_line(context.line_start, context.line_start + state->column_limit);
	}
	state->glyphs.back().pos = {context.line_length++, context.line};

	context.line_start += state->column_limit;
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
	if (context.line_length == state->column_limit && chr == ' ') {
		handle_newline(context);
	}
	else {
		state->glyphs.push_back({{context.line_length, context.line}, context.right_aligned, chr, context.text_color, context.bg_color});

		if (state->glyphs.size() - context.text_start > 1) {
			const char prev{state->glyphs[state->glyphs.size() - 2].chr};
			if (prev == ' ' && chr != ' ') {
				context.word_start = state->glyphs.size() - 1;
			}
		}

		if (++context.line_length > state->column_limit) {
			handle_column_limit(context);
		}
	}
}

void tr::gfx::debug_renderer::handle_newline(context& context)
{
	if (context.right_aligned) {
		right_align_line(context.line_start, state->glyphs.size());
	}

	context.line_length = 0;
	context.word_start = state->glyphs.size();
	context.line_start = state->glyphs.size();
	++context.line;
}

void tr::gfx::debug_renderer::handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, context& context,
													  rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	switch (*it) {
	case 'b':
		if (std::next(it) != end && std::isdigit(*++it) && std::uint8_t(*it - '0') < extra_colors.size()) {
			context.bg_color = extra_colors[*it - '0'];
		}
		break;
	case 'B':
		context.bg_color = bg_color;
		break;
	case 'c':
		if (std::next(it) != end && std::isdigit(*++it) && std::uint8_t(*it - '0') < extra_colors.size()) {
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
	const std::size_t old_size{state->glyphs.size()};
	context context{state->left_line, right, text_color, bg_color, 0, old_size, old_size, old_size};

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

//

void tr::gfx::debug_renderer::initialize(float scale, std::uint8_t column_limit)
{
	state.emplace();
	set_scale(scale);
	set_column_limit(column_limit);
}

bool tr::gfx::debug_renderer::active()
{
	return state.has_value();
}

void tr::gfx::debug_renderer::shut_down()
{
	state.reset();
}

//

void tr::gfx::debug_renderer::set_scale(float scale)
{
	state->pipeline.vertex_shader().set_uniform(1, scale);
}

void tr::gfx::debug_renderer::set_column_limit(std::uint8_t columns)
{
	state->column_limit = columns;
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
		write_right(std::format("{:<15}", name));
	}
	write_right(format_duration("MIN: ", benchmark.min()), benchmark.min() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("AVG: ", benchmark.avg()), benchmark.avg() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(format_duration("MAX: ", benchmark.max()), benchmark.max() < limit ? TEXT_COLOR : ALT_COLOR);
}

void tr::gfx::debug_renderer::newline_left()
{
	++state->left_line;
}

void tr::gfx::debug_renderer::newline_right()
{
	++state->right_line;
}

void tr::gfx::debug_renderer::draw()
{
	if (!state->glyphs.empty()) {
		set_render_target(backbuffer_render_target());
		if (current_renderer() != DEBUG_RENDERER) {
			set_renderer(DEBUG_RENDERER);
			set_blend_mode(ALPHA_BLENDING);
			set_shader_pipeline(state->pipeline);
			set_vertex_format(state->format);
			set_vertex_buffer(state->vbuffer, 0, 0);
		}

		if (static_cast<std::size_t>(state->sbuffer.array_capacity()) < state->glyphs.size() * sizeof(glyph)) {
			std::intptr_t cap{static_cast<std::intptr_t>(std::bit_ceil(state->glyphs.size() * sizeof(glyph)))};
			state->sbuffer = shader_buffer{0, cap, shader_buffer::access::WRITE_ONLY};
			if (debug()) {
				state->sbuffer.set_label("(tr) Debug Renderer Glyph Buffer");
			}
		}
		state->sbuffer.set_array(state->glyphs);
		state->pipeline.vertex_shader().set_uniform(0, static_cast<glm::vec2>(backbuffer_size()));
		state->pipeline.vertex_shader().set_storage_buffer(0, state->sbuffer);
		draw_instances(primitive::TRI_FAN, 0, 4, static_cast<int>(state->glyphs.size()));
		state->glyphs.clear();
	}

	state->left_line = 0;
	state->right_line = 0;
}