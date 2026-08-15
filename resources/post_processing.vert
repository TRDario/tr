#version 450

layout(location = 0) in vec2 position;

layout(location = 0) out vec2 output_uv;
out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
    output_uv = (position + vec2(1, -1)) / vec2(2, -2);
    gl_Position = vec4(position, 0, 1);
}