#version 450

// Vertex Data
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 0) out vec4 frag_color;

// Object Data
struct Object
{
	vec2 translation;
};

layout (std430, set = 0, binding = 1) readonly buffer ObjectData
{
	Object objects[];
};
layout (push_constant) uniform PushConstants
{
	uint object_index;
};


void main()
{
	Object obj = objects[object_index];

	gl_Position = vec4(
		in_position.x + obj.translation.x,
		in_position.y + obj.translation.y,
		in_position.z,
		1.0
	);
	frag_color = vec4(in_color, 1.0);
}