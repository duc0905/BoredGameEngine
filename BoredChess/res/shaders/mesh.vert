#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_Normal;

out vec2 v_UV;
out vec3 v_Normal;

void main()
{
    v_UV = a_UV;
    v_Normal = a_Normal;
    gl_Position = vec4(a_Pos, 1.0);
}