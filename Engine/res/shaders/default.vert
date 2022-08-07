#version 330 core

layout(location = 0) in vec3 a_Position;

uniform vec4 u_Color;
uniform mat4 u_Model;

out vec4 v_Color;

void main()
{
	gl_Position = u_Model * vec4(a_Position, 1.0f);
	// v_Color = vec4(a_Position, 1.0f);
	v_Color = u_Color;
}