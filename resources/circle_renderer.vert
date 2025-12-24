#version 450

layout(location = 0) uniform mat4 transform;
layout(location = 1) uniform float render_scale;

layout(location = 0) in vec2 relative_vertex_position;
layout(location = 1) in vec2 circle_position;
layout(location = 2) in float fill_radius;
layout(location = 3) in float outline_thickness;
layout(location = 4) in vec4 fill_color;
layout(location = 5) in vec4 outline_color;

layout(location = 0) out vec2 output_offset_from_center;
layout(location = 1) flat out float output_outline_start;
layout(location = 2) flat out vec4 output_fill_color;
layout(location = 3) flat out vec4 output_outline_color;
out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	const float half_outline_thickness = outline_thickness / 2;
	const float radius = fill_radius + half_outline_thickness;
	const float half_bounding_box_width = radius + (1 / render_scale);

	output_offset_from_center = (relative_vertex_position * 2 - 1) * (half_bounding_box_width / radius);
	output_outline_start = (fill_radius - half_outline_thickness) / radius;
	output_fill_color = fill_color;
	output_outline_color = outline_color;
	gl_Position = transform * vec4(circle_position + half_bounding_box_width * 2 * (relative_vertex_position - 0.5), 0, 1);
}