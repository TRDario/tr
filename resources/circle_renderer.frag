#version 450

layout(location = 0) in vec2 offset_from_center;
layout(location = 1) flat in float outline_start;
layout(location = 2) flat in vec4 fill_color;
layout(location = 3) flat in vec4 outline_color;

layout(location = 0) out vec4 output_color;

void main()
{
	const float distance_from_center = length(offset_from_center);
	const float transition_width = fwidth(distance_from_center);
	const float half_transition_width = transition_width / 2;
	const vec4 background_color = vec4(outline_color.rgb, 0);

	if (distance_from_center < outline_start - half_transition_width) {
		output_color = fill_color;
	}
	else if (distance_from_center <= outline_start + half_transition_width) {
		output_color = mix(outline_color, fill_color, (outline_start - distance_from_center + half_transition_width) / transition_width);
	}
	else if (distance_from_center <= 1 - half_transition_width) {
		output_color = outline_color;
	}
	else if (distance_from_center <= 1 + half_transition_width) {
		output_color = mix(background_color, outline_color, (1 - distance_from_center + half_transition_width) / transition_width);
	}
	else {
		output_color = background_color;
	}
}