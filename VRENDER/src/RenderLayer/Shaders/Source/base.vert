#version 450

layout (location = 0) out vec4 frag_color;

layout(set = 0, binding = 0) uniform TriangleData
{
	vec4 vertices[6];
	vec4 colors[6];
} ubo;

void main()
{
	gl_Position = ubo.vertices[gl_VertexIndex];
	frag_color = ubo.colors[gl_VertexIndex];
}