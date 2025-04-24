#version 330 core

uniform sampler2D u_tex;
in vec2 v_uv;

void main() {
    gl_FragColor = texture(u_tex, v_uv);
    // gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
