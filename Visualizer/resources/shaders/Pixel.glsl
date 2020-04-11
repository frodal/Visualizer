#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

uniform mat4 u_MVP;

out vec4 v_color;

void main()
{
	v_color = vec4(a_color, 1.0);
	gl_Position = u_MVP * vec4(a_position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
	color = v_color;
};