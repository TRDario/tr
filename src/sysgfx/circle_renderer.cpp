#include "../include/tr/sysgfx/circle_renderer.hpp"

namespace tr::gfx {
	// Circle renderer instance attributes.
	template <> struct vertex_attributes<circle_renderer::circle> : unpacked_vertex_attributes<glm::vec2, float, float, rgba8, rgba8> {};
	// Circle renderer vertex format.
	constexpr std::array<vertex_binding, 2> CIRCLE_FORMAT_ATTRIBUTES{{
		{NOT_INSTANCED, vertex_attributes<glm::u8vec2>::list},
		{1, unpacked_vertex_attributes<glm::vec2, float, float, rgba8, rgba8>::list},
	}};
	// Circle renderer vertex shader.
	constexpr const char* CIRCLE_RENDERER_VERT_SRC{
		"#version 450\n#define L(l) layout(location=l)\nL(0)uniform mat4 t;L(1)uniform float R;L(0)in vec2 p;L(1)in vec2 c;L(2)in "
		"float r;L(3)in float T;L(4)in vec4 f;L(5)in vec4 o;out gl_PerVertex{vec4 gl_Position;};L(0)out vec2 u;L(1)flat out float "
		"s;L(2)flat out vec4 F;L(3)flat out vec4 O;void main(){float "
		"h=T/2,S=r+h+(1/R);gl_Position=t*vec4(c+S*2*(p-0.5),0,1);u=(p*2-1)*(S/(r+h));s=(r-h)/S;F=f;O=o;}"};
	// Circle renderer fragment shader.
	constexpr const char* CIRCLE_RENDERER_FRAG_SRC{
		"#version 450\n#define L(l) layout(location=l)\nL(0)in vec2 u;L(1)flat in float s;L(2)flat in vec4 f;L(3)flat in vec4 o;L(0)"
		"out vec4 c;void main(){float D=length(u),d=fwidth(D),h=d/2;if(D<s-h)c=f;else if(D<=s+h)c=mix(o,f,(s-D+h)/d);else "
		"if(D<=1-h)c=o;else if(D<=1+h)c=mix(vec4(o.rgb,0),o,(1-D+h)/d);else c=vec4(o.rgb,0);}"};
	// Circle renderer ID.
	constexpr u32 CIRCLE_RENDERER_ID{5};
} // namespace tr::gfx

//

tr::gfx::circle_renderer::circle_renderer(float render_scale)
	: m_pipeline{vertex_shader{CIRCLE_RENDERER_VERT_SRC}, fragment_shader{CIRCLE_RENDERER_FRAG_SRC}}
	, m_vertex_format{CIRCLE_FORMAT_ATTRIBUTES}
	, m_quad_vertices{std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
{
	TR_SET_LABEL(m_pipeline, "(tr) Circle Renderer Pipeline");
	TR_SET_LABEL(m_pipeline.vertex_shader(), "(tr) Circle Renderer Vertex Shader");
	TR_SET_LABEL(m_pipeline.fragment_shader(), "(tr) Circle Renderer Fragment Shader");
	TR_SET_LABEL(m_vertex_format, "(tr) Circle Renderer Vertex Format");
	TR_SET_LABEL(m_shader_circles, "(tr) Circle Renderer Circle Buffer");
	TR_SET_LABEL(m_quad_vertices, "(tr) Circle Renderer Quad Buffer");

	set_render_scale(render_scale);
}

//

void tr::gfx::circle_renderer::set_render_scale(float render_scale)
{
	m_pipeline.vertex_shader().set_uniform(1, render_scale);
}

void tr::gfx::circle_renderer::set_default_transform(const glm::mat4& mat)
{
	m_default_transform = mat;
}

void tr::gfx::circle_renderer::set_default_layer_transform(int layer, const glm::mat4& mat)
{
	m_layers[layer].transform = mat;
}

void tr::gfx::circle_renderer::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	m_layers[layer].blend_mode = blend_mode;
}

//

void tr::gfx::circle_renderer::add_circle(int layer, const tr::circle& circle, rgba8 color)
{
	m_layers[layer].circles.emplace_back(circle.c, circle.r, 0, color, rgba8{color.r, color.b, color.b, 0});
}

void tr::gfx::circle_renderer::add_circle_outline(int layer, const tr::circle& circle, float outline_thickness, rgba8 color)
{
	m_layers[layer].circles.emplace_back(circle.c, circle.r, outline_thickness, rgba8{color.r, color.b, color.b, 0}, color);
}

void tr::gfx::circle_renderer::add_outlined_circle(int layer, const tr::circle& circle, float outline_thickness, rgba8 fill_color,
												   rgba8 outline_color)
{
	m_layers[layer].circles.emplace_back(circle.c, circle.r, outline_thickness, fill_color, outline_color);
}

//

void tr::gfx::circle_renderer::setup_context()
{
	if (active_renderer != CIRCLE_RENDERER_ID) {
		active_renderer = CIRCLE_RENDERER_ID;
		set_face_culling(false);
		set_depth_test(false);
		set_shader_pipeline(m_pipeline);
		set_blend_mode(m_last_blend_mode);
		set_vertex_format(m_vertex_format);
		set_vertex_buffer(m_quad_vertices, 0, 0);
	}
}

void tr::gfx::circle_renderer::setup_draw_call_state(const glm::mat4& transform, const blend_mode& blend_mode)
{
	if (m_last_transform != transform) {
		m_last_transform = transform;
		m_pipeline.vertex_shader().set_uniform(0, m_last_transform);
	}

	if (m_last_blend_mode != blend_mode) {
		m_last_blend_mode = blend_mode;
		set_blend_mode(m_last_blend_mode);
	}
}

void tr::gfx::circle_renderer::draw_layers(std::map<int, layer>::iterator first, std::map<int, layer>::iterator last,
										   const render_target& target)
{
	if (first == last) {
		return;
	};

	setup_context();
	set_render_target(target);

	std::vector<circle> circles;
	for (const auto& [priority, layer] : std::ranges::subrange{first, last}) {
		circles.insert(circles.end(), layer.circles.begin(), layer.circles.end());
	}
	m_shader_circles.set(circles);

	ssize offset{0};
	for (const auto& [priority, layer] : std::ranges::subrange{first, last}) {
		setup_draw_call_state(layer.transform.has_value() ? *layer.transform : m_default_transform, layer.blend_mode);
		set_vertex_buffer(m_shader_circles, 1, offset);
		draw_instances(primitive::TRI_FAN, 0, 4, layer.circles.size());
		offset += layer.circles.size();
	}
	m_layers.erase(first, last);
}

//

void tr::gfx::circle_renderer::draw_layer(int layer, const render_target& target)
{
	const auto range{m_layers.equal_range(layer)};
	draw_layers(range.first, range.second, target);
}

void tr::gfx::circle_renderer::draw_up_to_layer(int max_layer, const render_target& target)
{
	draw_layers(m_layers.begin(), m_layers.upper_bound(max_layer), target);
}

void tr::gfx::circle_renderer::draw(const render_target& target)
{
	draw_layers(m_layers.begin(), m_layers.end(), target);
}