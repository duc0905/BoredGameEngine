#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_UV;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjectionMat;

out vec2 v_TexCoords;
out vec3 v_normal;
out vec3 v_FragPos;

void main()
{
	gl_Position = u_ProjectionMat * u_ViewMat * u_ModelMat * vec4(a_Position, 1.0f);

	v_TexCoords = a_UV.xy;
	v_normal = a_Normal;
	v_FragPos = vec3(u_ModelMat * vec4(a_Position, 1.0));
}