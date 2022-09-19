#version 330 core

in vec2 v_TexCoords;
uniform int u_ActorID;

layout (location = 0) out vec4 color;
layout (location = 1) out int actorID;

uniform sampler2D sampler;
uniform vec3 u_ambientColor = vec3(1.0, 0.0, 0.0);
uniform float u_ambientStrength = 0.5;

void main()
{
	vec3 ambient = u_ambientColor * u_ambientStrength;
	color = vec4(ambient, 1.0) * texture2D(sampler, v_TexCoords);
	actorID = u_ActorID;
}