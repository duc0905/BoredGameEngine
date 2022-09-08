#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec3 a_Color;

out vec2 v_Pos;
out vec3 v_Color;

void main()
{
	gl_Position = vec4(a_Pos, 0.0, 1.0);
	v_Pos = a_Pos;
	v_Color = a_Color;
}